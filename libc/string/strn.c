#include <string.h>

char* strncat(char* __restrict dest, const char* __restrict src, size_t len){
    size_t pos = strlen(dest);
    size_t i = 0;

    while(src[i] && i<len) dest[pos++] = src[i++];
    return dest;
}

int strncmp(const char* __restrict ptr1, const char* __restrict ptr2, size_t len){
    size_t i = 0;

    while(ptr1[i] && i<len){
        if(((char*)ptr1)[i]>((char*)ptr2)[i]){
            return 1;
        } else if(((char*)ptr1)[i]<((char*)ptr2)[i]){
            return -1;
        }
        i++;
    }

    return 0;
}

char* strncpy(char* __restrict dest, const char* __restrict src, size_t len){
    size_t i = 0;
    while(src[i] && i<len) dest[i] = src[i];

    return dest;
}
