#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* __restrict dest, const void* __restrict src, size_t len);
void* memmove(void* __restrict dest, const void* __restrict src, size_t len);
void* memset(void* buf, int val, size_t len);
size_t strlen(const char* str);


#ifdef __cplusplus
}
#endif

#endif
