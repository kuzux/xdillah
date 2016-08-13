#include <string.h>

const char* strpbrk(const char* __restrict str1, const char* __restrict str2){
    size_t i,j;
    i = 0;

    while(str1[i]){
        j = 0;
        while(str2[j]){
            if(str1[i]==str2[j]){
                return str1+i;
            }
            j++;
        }
        i++;
    }

    return NULL;
}
