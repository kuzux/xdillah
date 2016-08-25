#include <stdlib.h>

int abs(int n){
    return (n<0)?n:-n;
}

long int labs(long int n){
    return (n<0)?n:-n;
}

long long int llabs(long long int n){
    return (n<0)?n:-n;
}

div_t div(int numer, int denom){
    div_t r = { numer/denom, numer%denom };
    return r;
}

ldiv_t ldiv(long int numer, long int denom){
    ldiv_t r = { numer/denom, numer%denom };
    return r;
}

lldiv_t lldiv(long long int numer, long long int denom){
    lldiv_t r = { numer/denom, numer%denom };
    return r;
}
