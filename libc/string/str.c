#include <string.h>

size_t strlen(const char* str){
    size_t res = 0;
    while(str[res]) res++;

    return res;
}

char* strcat (char* __restrict dest, const char* __restrict src){
    size_t pos = strlen(dest);
    size_t i = 0;

    while(src[i]) dest[pos++] = src[i++];
    return dest;    
}

const void* strchr(const char* ptr, int value){
    size_t i = 0;

    while(ptr[i]){
        if(((uint8_t*)ptr)[i]==(uint8_t)value){
            return ptr+i;
        }
        i++;
    }

    return NULL;
}

int strcmp(const char* __restrict ptr1, const char* __restrict ptr2){
    size_t i = 0;

    while(ptr1[i]){
        if(((char*)ptr1)[i]>((char*)ptr2)[i]){
            return 1;
        } else if(((char*)ptr1)[i]<((char*)ptr2)[i]){
            return -1;
        }
        i++;
    }

    return 0;
}

int strcoll(const char* __restrict ptr1, const char* __restrict ptr2){
    // todo: add locale support
    return strcmp(ptr1, ptr2);
}

char* strcpy(char* __restrict dest, const char* __restrict src){
    size_t i = 0;
    while(src[i]) dest[i] = src[i];

    return dest;
}

size_t strcspn(const char* __restrict str1, const char* __restrict str2){
    size_t i,j;
    i = 0;

    while(str1[i]){
        j = 0;
        while(str2[j]){
            if(str1[i]==str2[j]) return i;
        }
    }

    return 0;
}

char* strerror(int errcode){
    // todo: implement error codes
    return NULL;
}

const char* strrchr(const char* ptr, int value){
    size_t i = 0;
    int res = -1;

    while(ptr[i]){
        if(((uint8_t*)ptr)[i]==(uint8_t)value) res = i;
        i++;
    }

    if(res!=-1) return ptr+res;
    else return NULL;
}

size_t strspn(const char* __restrict str1, const char* __restrict str2){
    size_t i, j;
    char* res1 = NULL;
    char* res2 = NULL;

    i = 0;
    while(str1[i]){
        j = 0;
        while(str2[j]){
            if(str1[i]==str2[j]){
                if(!res1) res1 = str1+i;
                res2 = str1+i;
            }
            j++;
        }
        i++;
    }

    return res2 - res1;
}

const char* strstr(const char* __restrict str1, const char* __restrict str2){
    size_t i,j;

    i = 0;
    while(str1[i]){
        j = 0;
        while(str2[j]){
            if(str1[i]!=str2[j]) goto strstr_fail;
            j++;
        }

        return str1+i;
        strstr_fail:
        i++;
    }

    return NULL;
}

size_t strxfrm(char* __restrict destination, const char* __restrict source, size_t num){
    // todo: this function needs locale support to implement
    return 0;
}
