#ifndef _TTY_H
#define _TTY_H

#include <stdint.h>
#include <stddef.h>

void tty_init(void);
void tty_putch(char c);
void tty_puts(char* s);
void tty_putdec(uint32_t n);
void tty_puthex(uint32_t n);
void tty_putxy(char c, int i, int j);

#endif