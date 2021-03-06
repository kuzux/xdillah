#ifndef _STDIO_H
#define _STDIO_H

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif
 
int printf(const char*, ...);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif
 
#endif
