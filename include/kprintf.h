#ifndef _KPRINTF_H
#define _KPRINTF_H

#include <stddef.h>
#include <stdarg.h>
#include <tty.h>

size_t kprintf(const char* restrict fmt, ...);

#endif
