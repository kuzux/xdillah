#ifndef _STDINT_H
#define _STDINT_H

typedef __INT8_TYPE__ int8_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef __UINT32_TYPE__ size_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

#define INT8_MAX 0x7F
#define INT8_MIN (-INT8_MAX-1)
#define INT16_MAX 0x7FFF
#define INT16_MIN (-INT16_MAX-1)
#define INT32_MAX 0x7FFFFFL
#define INT32_MIN (-INT32_MAX-1L)
#define INT64_MAX 0x7FFFFFFFLL
#define INT64_MIN (-INT64_MAX-1LL)
#define UINT8_MAX 0xFFU
#define UINT16_MAX 0xFFFFU
#define UINT32_MAX 0xFFFFFFUL
#define UINT64_MAX 0xFFFFFFFFULL

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST8_MIN INT8_MIN
#define UINT_FAST8_MAX UINT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST16_MIN INT16_MIN
#define UINT_FAST16_MAX UINT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST32_MIN INT32_MIN
#define UINT_FAST32_MAX UINT32_MAX
#define INT_FAST64_MAX INT64_MAX
#define INT_FAST64_MIN INT64_MIN
#define UINT_FAST64_MAX UINT64_MAX

#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX

#endif