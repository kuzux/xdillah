#include <stdlib.h>
#include <string.h>

void* bsearch(const void* key, const void* base,
              size_t num, size_t size,
              int (*compar)(const void*,const void*)){
    size_t curr = num/2;

    int res = compar(base+size*curr, key);
    if(num <= 0) return NULL;

    if(res>0){
        return bsearch(key, base, num/2, size, compar);
    } else if(res<0){
        return bsearch(key, base+size*curr, num/2, size, compar);
    } else {
        return base+size*curr;
    }
}

void* median3(void* a, void* b, void* c,
              int (*compar)(const void*,const void*)){
    int res_ab = compar(a, b);
    int res_bc = compar(b, c);
    int res_ac = compar(a, c);

    if(res_ab > 0 && res_ac > 0){
        if(res_bc > 0){
            return b;
        } else {
            return c;
        }
    } else if(res_ab > 0 && res_ac < 0){
            return a;
    } else {
        if(res_bc > 0){
            return c;
        } else {
            return b;
        }
    }
}

void isort(void* base, size_t num, size_t size,
           int (*compar)(const void*,const void*)){
    size_t i;

    int j;

    int res;

    void* x = malloc(size);
    for(i=0; i<num; i++){
        memcpy(x, base+i*size, size);
        j = i - 1;

        res = compar(base+j*size, x);
        while(j>=0 && res > 0){
            memcpy(base+(j+1)*size, base+j*size, size);
            j--;
        }

        memcpy(base+(j+1)*size, x, size);
    }
}

int qpart(void* base, size_t num, size_t size, void* pivot,
           int (*compar)(const void*,const void*)){
    void* i;
    void* j;
    void* tmp = malloc(size);
    i = base - size;
    j = base + num*size;

    int ires, jres;

    for(;;){
        do{
            i += size;
            ires = compar(i, pivot);
        } while(ires < 0);

        do{
            j -= size;
            jres = compar(j, pivot);
        } while(jres > 0);

        if(i>=j){
            return (j-base)/size;
        }

        memcpy(tmp, j, size);
        memcpy(j, i, size);
        memcpy(i, tmp, size);
    }
}

void qsort(void* base, size_t num, size_t size,
           int (*compar)(const void*,const void*)){

    if(num<15) return isort(base, num, size, compar);

    size_t mid=size*(num/2);
    size_t end=size*(num-1);
    
    // returns the median pointer
    void* res = median3(base, base+mid, base+end, compar);

    int pivotidx = qpart(base, num, size, res, compar);
    
    qsort(base, pivotidx, size, compar);
    qsort(base+size*pivotidx, num-pivotidx-1, size, compar);
}
