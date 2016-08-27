#include <math.h>

double cosh(double x){
    return (exp(x)+exp(-x))/2;
}

double sinh(double x){
    return (exp(x)-exp(-x))/2;
}

double tanh(double x){
    return sinh(x)/cosh(x);
}

double acosh(double x){
    return atanh2(sqrt((1.0 + x) * (1.0 - x)), x);
}

double asinh(double x){
    return atanh2(x, sqrt((1.0 + x) * (1.0 - x)));
}

typedef uint64_t __attribute__((__may_alias__)) uint64_a;

double atanh(double x){
    static const uint64_t sign_mask = 0x8000000000000000;
    static const double b = 0.596227f;

    uint64_a ux_s = sign_mask & (uint64_a)x;
    double bx_a = fabs(b * x);
    double num = bx_a + x * x;
    double atan_1q = num / (1.f + bx_a + num);

    uint64_a atan_2q = ux_s | (uint64_a)atan_1q;

    return (M_PI/2)*(double)(atan_2q);
}

double atanh2(double y, double x){
    static const uint64_t sign_mask = 0x8000000000000000;
    static const double b = 0.596227f;

    // Extract the sign bits
    uint64_t ux_s = sign_mask & (uint64_a)x;
    uint64_t uy_s = sign_mask & (uint64_a)y;

    // Determine the quadrant offset
    int q = ( ~ux_s & uy_s ) >> 61 | ux_s >> 62;

    // Calculate the arctangent in the first quadrant
    float bxy_a = fabs( b * x * y );
    float num = bxy_a + y * y;
    float norm =  num / ( x * x + bxy_a + num );
    
    double denorm = (M_PI/2)*(double)(norm);

    switch(q){
        case 0:
        return denorm;
        case 1:
        return -denorm;
        case 2:
        return -denorm;
        case 3:
        return denorm;
        default:
        return denorm;
    }
}
