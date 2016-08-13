#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

int memcmp(const void* __restrict ptr1, const void* __restrict ptr2, size_t len);
void* memcpy(void* __restrict dest, const void* __restrict src, size_t len);
void* memmove(void* __restrict dest, const void* __restrict src, size_t len);
const void* memchr(const void* ptr, int value, size_t len);
void* memset(void* buf, int val, size_t len);

size_t strlen(const char* str);
char* strcat(char* __restrict dest, const char* __restrict src);
const void* strchr(const char* ptr, int value);
int strcmp(const char* __restrict ptr1, const char* __restrict ptr2);
int strcoll(const char* __restrict ptr1, const char* __restrict ptr2);
char* strcpy(char* __restrict dest, const char* __restrict src);
size_t strcspn(const char* __restrict str1, const char* __restrict str2);
char* strerror(int errcode);

char* strncat(char* __restrict dest, const char* __restrict src, size_t len);
int strncmp(const char* __restrict ptr1, const char* __restrict ptr2, size_t len);
char* strncpy(char* __restrict dest, const char* __restrict src, size_t len);

const char* strpbrk(const char* __restrict str1, const char* __restrict str2);

const char* strrchr(const char* ptr, int value);
size_t strspn(const char* __restrict str1, const char* __restrict str2);
const char* strstr(const char* __restrict str1, const char* __restrict str2);
size_t strxfrm(char* __restrict destination, const char* __restrict source, size_t num);

char* strtok (char* __restrict str, const char* __restrict delim);

#ifdef __cplusplus
}
#endif

#endif
