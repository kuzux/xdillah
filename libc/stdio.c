#include <stdio.h>
#include <ctype.h>

#if defined(__is_xdillah_kernel)
#include <kernel/tty.h>
#include <kernel/serial.h>
#endif

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

void putdec(uint32_t n, char* buf, printf_options_t opts){
    if (n == 0){
        buf[0] = '0';
        return;
    }

    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    } else if(opts.sign&&n>0) {
        buf[idx] = '+';
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

void putoct(uint32_t n, char* buf, printf_options_t opts){
    if (n == 0){
        buf[0] = '0';
        return;
    }

    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    } else if(opts.sign&&n>0) {
        buf[idx] = '+';
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

void puthex(uint32_t n, char* buf, printf_options_t opts){
    int32_t tmp;
    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    } else if(opts.sign&&n>0) {
        buf[idx] = '+';
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

void puthex_caps(uint32_t n, char* buf, printf_options_t opts){
    int32_t tmp;
    uint32_t idx = 0;

    if(n<0){
        buf[idx] = '-';
        idx++;
    } else if(opts.sign&&n>0) {
        buf[idx] = '+';
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
    opts->precision = 1;

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

#define PRINTF_INT(var, type, fn) var = (type)va_arg(params, type); \
    fn(var, numbuf, opts); \
    res += put_pad(numbuf, opts); \
    break;

#define PRINTF_LEN(var, type) (var) = (type*)va_arg(params, type*); \
    *(var) = res; \
    break;

int vprintf(const char* fmt, va_list params){
    printf_options_t opts;
    init_opts(&opts);

    size_t res = 0;

    int n;
    char c;
    const char* s;
    void* vp;
    unsigned int u;

    short sh;
    long l;
    long long ll;
    intmax_t im;
    size_t sz;
    ptrdiff_t pd;

    unsigned char uc;
    unsigned short ush;
    unsigned long ul;
    unsigned long long ull;
    uintmax_t uim;

    int* ip;
    char* cp;
    short* shp;
    long* lp;
    long long* llp;
    intmax_t* imp;
    size_t* szp;
    ptrdiff_t* pdp;

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

        if(*fmt=='\0') break;

        // parse the minwidth
        if(*fmt == '*'){
            n = (int)va_arg(params, int);
            opts.minwidth = n;
            fmt++;
            goto precision_parse;
        }
        if(*fmt=='\0') break;
        while(isdigit(*fmt)){
            opts.minwidth *= 10;
            opts.minwidth += (*fmt-'0');
            fmt++;
        }
        precision_parse:
        if(*fmt=='\0') break;

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

            if(*fmt=='\0') break;
            while(isdigit(*fmt)){
                opts.precision *= 10;
                opts.precision += (*fmt-'0');
                fmt++;
            }
        }
        length_parse:
        if(*fmt=='\0') break;

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
        
        if(*fmt=='\0') break;

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
                switch(opts.lengthmod){
                    case 'H':
                    PRINTF_INT(c, char, putdec)
                    case 'h':
                    PRINTF_INT(sh, short, putdec)
                    case 'l':
                    PRINTF_INT(l, long, putdec)
                    case 'm':
                    PRINTF_INT(ll, long long, putdec)
                    case 'j':
                    PRINTF_INT(im, intmax_t, putdec)
                    case 'z':
                    PRINTF_INT(sz, size_t, putdec)
                    case 't':
                    PRINTF_INT(pd, ptrdiff_t, putdec)
                    default:
                    PRINTF_INT(n, int, putdec)
                }

                break;
            case 'u':
                switch(opts.lengthmod){
                    case 'H':
                    PRINTF_INT(uc, unsigned char, putdec)
                    case 'h':
                    PRINTF_INT(ush, unsigned short, putdec)
                    case 'l':
                    PRINTF_INT(ul, unsigned long, putdec)
                    case 'm':
                    PRINTF_INT(ull, unsigned long long, putdec)
                    case 'j':
                    PRINTF_INT(uim, uintmax_t, putdec)
                    case 'z':
                    PRINTF_INT(sz, size_t, putdec)
                    case 't':
                    PRINTF_INT(pd, ptrdiff_t, putdec)
                    default:
                    PRINTF_INT(u, unsigned int, putdec)
                }

                break;
            case 'x':
                switch(opts.lengthmod){
                    case 'H':
                    PRINTF_INT(uc, unsigned char, puthex)
                    case 'h':
                    PRINTF_INT(ush, unsigned short, puthex)
                    case 'l':
                    PRINTF_INT(ul, unsigned long, puthex)
                    case 'm':
                    PRINTF_INT(ull, unsigned long long, puthex)
                    case 'j':
                    PRINTF_INT(uim, uintmax_t, puthex)
                    case 'z':
                    PRINTF_INT(sz, size_t, puthex)
                    case 't':
                    PRINTF_INT(pd, ptrdiff_t, puthex)
                    default:
                    PRINTF_INT(u, unsigned int, puthex)
                }

                break;
            case 'X':
                switch(opts.lengthmod){
                    case 'H':
                    PRINTF_INT(uc, unsigned char, puthex_caps)
                    case 'h':
                    PRINTF_INT(ush, unsigned short, puthex_caps)
                    case 'l':
                    PRINTF_INT(ul, unsigned long, puthex_caps)
                    case 'm':
                    PRINTF_INT(ull, unsigned long long, puthex_caps)
                    case 'j':
                    PRINTF_INT(uim, uintmax_t, puthex_caps)
                    case 'z':
                    PRINTF_INT(sz, size_t, puthex_caps)
                    case 't':
                    PRINTF_INT(pd, ptrdiff_t, puthex_caps)
                    default:
                    PRINTF_INT(u, unsigned int, puthex_caps)
                }

                break;
            case 'o':
                switch(opts.lengthmod){
                    case 'H':
                    PRINTF_INT(uc, unsigned char, putoct)
                    case 'h':
                    PRINTF_INT(ush, unsigned short, putoct)
                    case 'l':
                    PRINTF_INT(ul, unsigned long, putoct)
                    case 'm':
                    PRINTF_INT(ull, unsigned long long, putoct)
                    case 'j':
                    PRINTF_INT(uim, uintmax_t, putoct)
                    case 'z':
                    PRINTF_INT(sz, size_t, putoct)
                    case 't':
                    PRINTF_INT(pd, ptrdiff_t, putoct)
                    default:
                    PRINTF_INT(u, unsigned int, putoct)
                }

                break;
            case 's':
                s = (const char*)va_arg(params, const char*);
                res += puts(s);

                break;
            case 'p':
                vp = (void*)va_arg(params, void*);
                puthex((uint32_t)vp, numbuf, opts);
                res += put_pad(numbuf, opts);

                break;
            case 'n':
                switch(opts.lengthmod){
                    case 'H':
                    PRINTF_LEN(cp, char*)
                    case 'h':
                    PRINTF_LEN(shp, short*)
                    case 'l':
                    PRINTF_LEN(lp, long*)
                    case 'm':
                    PRINTF_LEN(llp, long long*)
                    case 'j':
                    PRINTF_LEN(imp, intmax_t*)
                    case 'z':
                    PRINTF_LEN(szp, size_t*)
                    case 't':
                    PRINTF_LEN(pdp, ptrdiff_t*)
                    default:
                    PRINTF_LEN(ip, int*)
                }

                break;
            default:
                putchar(*fmt);
                res++;

                break;
        }
    }

    return res;
}

int printf(const char* fmt, ...){
    va_list params;
    va_start(params, fmt);

    size_t res = 0;
    res = vprintf(fmt, params);

    va_end(params);

    return res;
}
