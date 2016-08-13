#include <stdlib.h>

#define a 1103515245
#define c 12345

static int prev;

int rand(void){
    prev = (a*prev+c)%RAND_MAX;
    return prev;
}

void srand(unsigned int seed){
    prev = seed;
}
