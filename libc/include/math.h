#ifndef _MATH_H
#define _MATH_H

#include <stddef.h>
#include <stdint.h>

#define     M_E   2.7182818284590452354
#define     M_LOG2E   1.4426950408889634074 /* log_2 e */
#define     M_LOG10E   0.43429448190325182765   /* log_10 e */
#define     M_LN2   0.69314718055994530942  /* log_e 2 */
#define     M_LN10   2.30258509299404568402 /* log_e 10 */
#define     M_PI   3.14159265358979323846   /* pi */
#define     M_PI_2   1.57079632679489661923 /* pi/2 */
#define     M_PI_4   0.78539816339744830962 /* pi/4 */
#define     M_1_PI   0.31830988618379067154 /* 1/pi */
#define     M_2_PI   0.63661977236758134308 /* 2/pi */
#define     M_2_SQRTPI   1.12837916709551257390 /* 2/sqrt(pi) */
#define     M_SQRT2   1.41421356237309504880    /* sqrt(2) */
#define     M_SQRT1_2   0.70710678118654752440  /* 1/sqrt(2) */
#define     NAN   __builtin_nan("") 
#define     INFINITY   __builtin_inf()

double cos(double x);
double sin(double x);
double tan(double x);
double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double x, double y);

double cosh(double x);
double sinh(double x);
double tanh(double x);
double acosh(double x);
double asinh(double x);
double atanh(double x);

double exp(double x);
double frexp(double x, int* exp);
double ldexp(double x, int exp);
double log(double x);
double log10(double x);
double modf(double x, double* intpart);
double exp2(double x);
double expm1(double x);
double ilogb(double x);
double log1p(double x);
double log2(double x);
double scalbn(double x, int n);
double scalbln(double x, long n);

double pow(double b, double e);
double sqrt(double x);
double cbrt(double x);
double hypot(double x);

double erf(double x);
double erfc(double x);
double tgamma(double x);
double lgamma(double x);

double ceil(double x);
double floor(double x);
double fmod(double n, double d);
double trunc(double x);
double round(double x);
long lround(double x);
long long llround(double x);
double rint(double x);
long lrint(double x);
long long llrint(double x);
double nearbyint(double x);
double remainder(double n, double d);
double remquo(double n, double d, int* q);

double copysign(double x, double y);
double nan(const char* tagp);
double nextafter(double x, double y);
double nexttoward(double x, long double y);

double fdim(double x, double y);
double fmax(double x, double y);
double fmin(double x, double y);

double abs(double x);
double fabs(double x);
double fma(double x, double y, double z);

typedef float float_t;
typedef double double_t;

#endif
