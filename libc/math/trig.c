#include <math.h>

extern double _sin(double);
extern double _cos(double);

double cos(double x){
    return _cos(x);
}

double sin(double x){
    double sinx, cosx;
    asm("fsincos" : "=t" (cosx), "=u" (sinx) : "0" (x));
    return _sin(x);
}

double tan(double x){
    return sin(x)/cos(x);
}

double acos(double x){
    return atan2(sqrt((1.0 + x) * (1.0 - x)), x);
}

double asin(double x){
    return atan2(x, sqrt((1.0 + x) * (1.0 - x)));
}

typedef uint64_t __attribute__((__may_alias__)) uint64_a;

double atan(double x){
    // mostly copied from 
    // http://stackoverflow.com/questions/7378187/approximating-inverse-trigonometric-funcions
    static const uint64_t sign_mask = 0x8000000000000000LL;
    static const double b = 0.596227f;

    uint64_a ux_s = sign_mask & (uint64_a)x;
    double bx_a = fabs(b * x);
    double num = bx_a + x * x;
    double atan_1q = num / (1.f + bx_a + num);

    uint64_a atan_2q = ux_s | (uint64_a)atan_1q;

    return (M_PI/2)*(double)(atan_2q);
}

double atan2(double y, double x){
    // mostly copied from 
    // http://stackoverflow.com/questions/7378187/approximating-inverse-trigonometric-funcions
    static const uint64_t sign_mask = 0x8000000000000000LL;
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
