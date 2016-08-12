#include <string.h>
#include <stdbool.h>

static char* prev = NULL;

char* strtok (char* __restrict str, const char* __restrict delim){
    char* tmp;
    size_t i, j;

    if(str==NULL){
        i = 0;
        while(prev[i]){
            j = 0;
            while(delim[j]){
                if(prev[i]==delim[j]){
                    prev[i] = '\0';
                    tmp = prev;
                    prev += i+1;
                    return tmp;
                }
                j++;
            }
            i++;
        }
    } else {
        prev = str;
        strtok(NULL, delim);
    }

    return NULL;
}
