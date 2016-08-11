#include <string.h>

void* memcpy(void* __restrict dest, const void* __restrict src, size_t len){
    uint8_t* dstp = (uint8_t*)dest;
    uint8_t* srcp = (uint8_t*)src;

    size_t i;
    for(i=0;i<len;i++) dstp[i] = srcp[i];

    return dest;
}

void* memmove(void* __restrict dest, const void* __restrict src, size_t len){
    uint8_t* dstp = (uint8_t*)dest;
    uint8_t* srcp = (uint8_t*)src;

    size_t i;

    if(dest < src){
        for(i=0;i<len;i++)
            dstp[i] = srcp[i];
    } else {
        for(i=len;i>0;i--)
            dstp[i-1] = srcp[i-1];
    }

    return dest;
}

void* memset(void* buf, int val, size_t len){
    uint8_t* bufp = (uint8_t*) buf;

    size_t i;
    for(i=0;i<len;i++)
        bufp[i] = (uint8_t)val;

    return buf;
}

size_t strlen(const char* str){
    size_t res = 0;
    while(str[res]) res++;

    return res;
}
