#include <unistd.h>

#if defined(__is_xdillah_kernel)
#include <kernel/kheap.h>
#endif

int brk(void* addr){
    #if defined(__is_xdillah_kernel)
    return do_brk(addr);
    #else
    return -1;
    //todo: syscall
    #endif
}

uintptr_t getbrk(){
    #if defined(__is_xdillah_kernel)
    return do_getbrk();
    #else
    return -1;
    //todo: syscall
    #endif
}

static uintptr_t currbrk = NULL;

#define HEAP_INITIAL_SIZE 0x100000

void* sbrk(intptr_t delta){
    if(currbrk==NULL){
        currbrk = getbrk();
    }

    if(delta==0) return currbrk;
    currbrk += delta;

    int res = brk(currbrk);
    if(res<0) return res;
    return currbrk;
}
