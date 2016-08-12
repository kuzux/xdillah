#include <string.h>

int memcmp(const void* __restrict ptr1, const void* __restrict ptr2, size_t len){
    size_t i;

    for(i=0;i<len;i++){
        if(((char*)ptr1)[i]>((char*)ptr2)[i]){
            return 1;
        } else if(((char*)ptr1)[i]<((char*)ptr2)[i]){
            return -1;
        }
    }

    return 0;
}

void* memcpy(void* __restrict dest, const void* __restrict src, size_t len){
    uint8_t* dstp = (uint8_t*)dest;
    uint8_t* srcp = (uint8_t*)src;

    size_t i;
    // todo: implement duff's device or something 
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

const void* memchr(const void* ptr, int value, size_t len){
    size_t i;

    for(i=0;i<len;i++){
        if(((uint8_t*)ptr)[i]==(uint8_t)value) return ptr+i;
    }

    return NULL;
}

