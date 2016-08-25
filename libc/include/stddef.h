#ifndef _STDDEF_H
#define _STDDEF_H

#include <stdint.h>

typedef uint32_t size_t;
typedef int32_t ptrdiff_t;
typedef uint64_t max_align_t;
typedef void* nullptr_t;

#define NULL ((void*)0)

#define offsetof(t, e) ((size_t)&*(((t*)0)->e))

#endif
