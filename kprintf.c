#include <kprintf.h>
#include <tty.h>

size_t kprintf(const char* restrict fmt, ...){
    va_list params;
    va_start(params, fmt);

    size_t res = 0;

    int n;
    char c;
    const char* s;

    while (*fmt){
        if(*fmt != '%'){
            tty_putch(*fmt);
            res++;
            fmt++;

            continue;
        }

        fmt++;
        switch(*fmt){
            case 'd':
                n = (int)va_arg(params, int);
                res += tty_putdec(n);
                break;
            case 'x':
                n = (int)va_arg(params, int);
                res += tty_puthex(n);
                break;
            case 'c':
                c = (char)va_arg(params, int);
                tty_putch(c);
                res++;
                break;
            case 's':
                s = (const char*)va_arg(params, const char*);
                res += tty_puts(s);
                break;
            case '%':
                tty_putch('%');
                res++;
                break;
            default:
                break;
        }

        fmt++;
    }

    return res;
}
