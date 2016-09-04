#include <stdio.h>

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

size_t putdec(uint32_t n){
    if (n == 0){
        putchar('0');
        return 1;
    }

    if(n<0){
        putchar('-');
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
    c2[i--] = 0;
    int j = 0;
    while(i >= 0){
        c2[i--] = c[j++];
    }
    return (size_t) puts(c2);
}

size_t putoct(uint32_t n){
    if (n == 0){
        putchar('0');
        return 1;
    }

    if(n<0){
        putchar('-');
    }

    putchar("0");

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0){
        c[i] = '0' + acc%8;
        acc /= 8;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0){
        c2[i--] = c[j++];
    }
    return (size_t) puts(c2);
}

size_t puthex(uint32_t n){
    int32_t tmp;

    if(n<0){
        putchar('-');
    }

    puts("0x");
    size_t res = 0;

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        res++;
        if (tmp >= 0xA){
            noZeroes = 0;
            putchar(tmp-0xA+'a');
        }
        else{
            noZeroes = 0;
            putchar(tmp+'0');
        }
    }
  
    tmp = n & 0xF;
    res++;
    if (tmp >= 0xA){
        putchar(tmp-0xA+'a');
    }
    else{
        putchar(tmp+'0');
    }

    return res;
}

size_t puthex_caps(uint32_t n){
    int32_t tmp;

    if(n<0){
        putchar('-');
    }

    puts("0X");
    size_t res = 0;

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        res++;
        if (tmp >= 0xA){
            noZeroes = 0;
            putchar(tmp-0xA+'A');
        }
        else{
            noZeroes = 0;
            putchar(tmp+'0');
        }
    }
  
    tmp = n & 0xF;
    res++;
    if (tmp >= 0xA){
        putchar(tmp-0xA+'a');
    }
    else{
        putchar(tmp+'0');
    }

    return res;
}

int printf(const char* restrict fmt, ...){
    va_list params;
    va_start(params, fmt);

    size_t res = 0;

    int n;
    unsigned int u;
    char c;
    const char* s;
    void* vp;
    int* ip;

    char* p;

    while(*fmt){
        while(*fmt!='%'&&*fmt!='\0'){
            putchar(*fmt);
            fmt++;
            res++;
        }

        if(*fmt=='\0') break;

        fmt++;

        if(*fmt=='\0') break;

        switch(*fmt){
            case 'c':
                c = (char)va_arg(params, int);
                putchar(c);
                res++;
                break;
            case 'd':
            case 'i':
                n = (int)va_arg(params, int);
                res += putdec(n);
                break;
            case 'x':
                u = (unsigned int)va_arg(params, unsigned int);
                res += puthex(n);
                break;
            case 'X':
                u = (unsigned int)va_arg(params, unsigned int);
                res += puthex_caps(n);
                break;
            case 'o':
                u = (unsigned int)va_arg(params, unsigned int);
                res += putoct(n);
                break;
            case 'p':
                vp = (void*)va_arg(params, void*);
                res += puthex((uint32_t)p);
                break;
            case 's':
                s = (const char*)va_arg(params, const char*);
                res += puts(s);
                break;
            case 'n':
                ip = (int*)va_arg(params, int*);
                *ip = res;
                res += putdec(res);
            default:
                putchar(*fmt);
                res++;
                break;
        }

        fmt++;
    }

    va_end(params);

    return res;
}
