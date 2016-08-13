#ifndef _STDLIB_H
#define _STDLIB_H 1
 
#include <sys/cdefs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct div_t {
    int quot;
    int rem;
} div_t;

typedef struct ldiv_t {
    long int quot;
    long int rem;
} ldiv_t;

typedef struct lldiv_t {
    long long int quot;
    long long int rem;
} lldiv_t;

#define NULL 0
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define MB_CUR_MAX 65535
#define RAND_MAX 2147483647

double atof(const char* str);
int atoi(const char* str);
long atol(const char* str);
long long atoll(const char* str);

double strtod(const char* str, char** endptr);
float strtof(const char* str, char** endptr);
long double strtold(const char* str, char** endptr);
long strtol(const char* str, char** endptr);
long long strtoll(const char* str, char** endptr);
unsigned long strtoul(const char* str, char** endptr);
unsigned long long strtoull(const char* str, char** endptr);

int rand(void);
void srand(unsigned int seed);

void* calloc(size_t num, size_t size);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

__attribute__((__noreturn__))
void abort(void);
int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));
void exit(int status);
_Noreturn void quick_exit(int status);
void _Exit(int status);
char* getenv(const char* name);
int system(const char* command);

void* bsearch(const void* key, const void* base,
              size_t num, size_t size,
              int (*compar)(const void*,const void*));

void qsort(void* base, size_t num, size_t size,
           int (*compar)(const void*,const void*));

int abs(int n);
long int labs(long int n);
long long int llabs(long long int n);
div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);
lldiv_t lldiv(long long int numer, long long int denom);

typedef int wchar_t;

int mblen(const char* pmb, size_t max);
int mbtowc(wchar_t* pwc, const char* pmb, size_t max);
int wctomb(char* pmb, wchar_t wc);
size_t wcstombs(char* dest, const wchar_t* src, size_t max);
size_t mbstowcs(wchar_t* dest, const char* src, size_t max);

#ifdef __cplusplus
}
#endif
 
#endif
