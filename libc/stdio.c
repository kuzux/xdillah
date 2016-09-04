#include <stdio.h>
#include <ctype.h>

#if defined(__is_xdillah_kernel)
#include <kernel/tty.h>
#include <kernel/serial.h>
#endif

void sp(const char* restrict s){
    #if defined(__is_xdillah_kernel)
        puts_serial(s, COM1);
    #endif
}

int putchar(int ic){
    #if defined(__is_xdillah_kernel)
        char c = (char) ic;
        tty_putch(c);
    #else
        // todo: implement a system call
    #endif

    return ic;
}

int puts(const char* s){
    size_t res = 0;

    while (*s){
        putchar(*s);
        res++;
        s++;
    }

    return res;
}

void putdec(uint32_t n, char* buf){
    if (n == 0){
        buf[0] = '0';
        return;
    }

    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0){
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    buf[i--] = 0;
    int j = 0;
    while(i >= 0){
        buf[i--] = c[j++];
    }
}

void putoct(uint32_t n, char* buf){
    if (n == 0){
        buf[0] = '0';
        return;
    }

    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    }

    buf[idx] = '0';
    idx++;

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0){
        c[i] = '0' + acc%8;
        acc /= 8;
        i++;
    }
    c[i] = 0;

    buf[i--] = 0;
    int j = 0;
    while(i >= 0){
        buf[i--] = c[j++];
    }
}

void puthex(uint32_t n, char* buf){
    int32_t tmp;
    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    }

    buf[idx] = '0';
    idx++;
    buf[idx] = 'x';
    idx++;

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        if (tmp >= 0xA){
            noZeroes = 0;
            buf[idx] = (tmp-0xA+'a');
            idx++;
        }
        else{
            noZeroes = 0;
            buf[idx] = tmp+'0';
            idx++;
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA){
        buf[idx] = (tmp-0xA+'a');
        idx++;
    }
    else{
        buf[idx] = tmp+'0';
        idx++;
    }
}

void puthex_caps(uint32_t n, char* buf){
    int32_t tmp;
    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    }

    buf[idx] = '0';
    idx++;
    buf[idx] = 'X';
    idx++;

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        if (tmp >= 0xA){
            noZeroes = 0;
            buf[idx] = (tmp-0xA+'A');
            idx++;
        }
        else{
            noZeroes = 0;
            buf[idx] = tmp+'0';
            idx++;
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA){
        buf[idx] = (tmp-0xA+'A');
        idx++;
    }
    else{
        buf[idx] = tmp+'0';
        idx++;
    }
}

typedef struct printf_options{
    uint8_t justify;
    uint8_t sign;
    uint8_t space;
    uint8_t alternative;
    char padchar;
    int minwidth;
    int precision;
    char lengthmod;
    char format;
} printf_options_t;

void init_opts(printf_options_t* opts){
    // 0 = right justified, 1 = left justified
    opts->justify = 0;

    // 0 = only minus displayed, 1 = both signs displayed
    opts->sign = 0;

    // if the result of a signed conversion does not start with a sign 
    // character, or is empty, space is prepended to the result. It is 
    // ignored if + flag is present.
    opts->space = 0;

    // use the alternative implementation if present
    opts->alternative = 0;

    // it can be 0 as well
    opts->padchar = ' ';

    // minimum width to be padded to, if *, an integer parameter is read
    opts->minwidth = 0;

    // used for floating point precision and int minimum width to be printed
    opts->precision = -1;

    // the actual length modifier character. H for hh, m for ll
    opts->lengthmod = '\0';

    // the actual format character
    opts->format = '\0';
}

int put_pad(char* str, printf_options_t opts){
    uint32_t n = strlen(str);
    if(opts.minwidth < n){
        return puts(str);
    }

    int diff = opts.minwidth - n;
    uint32_t i, j;
    if(!opts.justify){
        for(i=0;i<diff;i++){
            putchar(opts.padchar);
        }
        for(j=0;j<n;j++){
            putchar(str[j]);
        }
    } else {
        for(j=0;j<n;j++){
            putchar(str[j]);
        }
        for(i=0;i<diff;i++){
            putchar(opts.padchar);
        }
    }
}

int printf(const char* restrict fmt, ...){
    va_list params;
    va_start(params, fmt);

    printf_options_t opts;
    init_opts(&opts);

    size_t res = 0;

    int n;
    char c;
    const char* s;
    void* vp;
    int* ip;
    unsigned int u;

    char numbuf[255];
    uint32_t i;
    for(i=0;i<255;i++){
        numbuf[i] = 0;
    }

    while(*fmt){
        while(*fmt!='%'&&*fmt!='\0'){
            putchar(*fmt);
            fmt++;
            res++;
        }

        if(*fmt=='\0') break;

        ASSERT(*fmt == '%');
        fmt++;

        if(*fmt=='\0') break;

        // parse the optional flags
        switch(*fmt){
            case '-':
                opts.justify = 1;
                fmt++;
                break;
            case '+':
                opts.sign = 1;
                fmt++;
                break;
            case ' ':
                opts.space = 1;
                fmt++;
                break;
            case '#':
                opts.alternative = 1;
                fmt++;
                break;
            case '0':
                opts.padchar = '0';
                fmt++;
                break;
            default:
                break;
        }

        // parse the minwidth
        if(*fmt == '*'){
            n = (int)va_arg(params, int);
            opts.minwidth = n;
            fmt++;
            goto precision_parse;
        }
        while(isdigit(*fmt)){
            opts.minwidth *= 10;
            opts.minwidth += (*fmt-'0');
            fmt++;
        }
        precision_parse:

        // parse the precision.
        // it is .* or .n where n is a number
        if(*fmt=='.'){
            fmt++;
            if(*fmt == '*'){
                n = (int)va_arg(params, int);
                opts.precision = n;
                fmt++;
                goto length_parse;
            }

            while(isdigit(*fmt)){
                opts.precision *= 10;
                opts.precision += (*fmt-'0');
                fmt++;
            }
        }
        length_parse:

        // parse the length character
        switch(*fmt){
            case 'j':
            case 'z':
            case 't':
            case 'L':
                opts.lengthmod = *fmt;
                fmt++;
                break;
            case 'h':
                fmt++;
                if(*fmt=='h'){
                    opts.lengthmod = 'H';
                    fmt++;
                } else {
                    opts.lengthmod = 'h';
                }
                break;
            case 'l':
                fmt++;
                if(*fmt=='l'){
                    opts.lengthmod = 'm';
                    fmt++;
                } else {
                    opts.lengthmod = 'l';
                }
                break;
            default:
                break;
        }

        opts.format = *fmt;
        fmt++;

        switch(opts.format){
            case 'c':
                c = (char)va_arg(params, int);
                putchar(c);
                res++;

                break;
            case 'd':
            case 'i':
                n = (int)va_arg(params, int);
                putdec(n, numbuf);
                res += put_pad(numbuf, opts);

                break;
            case 'u':
                u = (unsigned int)va_arg(params, unsigned int);
                putdec(u, numbuf);
                res += put_pad(numbuf, opts);

                break;
            case 'x':
                u = (unsigned int)va_arg(params, unsigned int);
                puthex(u, numbuf);
                res += put_pad(numbuf, opts);

                break;
            case 'X':
                u = (unsigned int)va_arg(params, unsigned int);
                puthex_caps(u, numbuf);
                res += put_pad(numbuf, opts);

                break;
            case 'o':
                u = (unsigned int)va_arg(params, unsigned int);
                putoct(u, numbuf);
                res += put_pad(numbuf, opts);

                break;
            case 's':
                s = (const char*)va_arg(params, const char*);
                res += puts(s);

                break;
            case 'p':
                vp = (void*)va_arg(params, void*);
                puthex((uint32_t)vp, numbuf);
                res += put_pad(numbuf, opts);

                break;
            case 'n':
                ip = (int*)va_arg(params, int*);
                *ip = res;

                break;
            default:
                putchar(*fmt);
                res++;

                break;
        }
    }

    va_end(params);

    return res;
}
