#include <kernel/brk.h>
#include <kernel/paging.h>
#include <assert.h>

// curr_dir (used in expand/contract) is defined at paging.h
heap_t* kheap;

void expand(heap_t* h, void* newaddr){
    uint32_t iaddr = (uint32_t)newaddr;

    ASSERT(h != NULL);
    ASSERT(iaddr > h->end);

    // page align the new break
    if((iaddr & 0xFFFFF000) != 0){
        iaddr &= 0xFFFFF000;
        iaddr += 0x1000;
    }

    ASSERT(iaddr <= h->max);

    // assume our previous end address is page aligned
    ASSERT((h->end & 0xFFFFF000) == 0);
    uint32_t i = 0;
    for(i=h->end;i<iaddr;i+=0x1000){
        alloc_frame(get_page(i, 1, curr_dir), h->supervisor, h->readonly);
    }

    h->end = iaddr;
}

void contract(heap_t* h, void* newaddr){
    uint32_t iaddr = (uint32_t)newaddr;

    ASSERT(iaddr < h->end);
    ASSERT(iaddr >= h->start + HEAP_MIN_SIZE);

    // page align the new break
    if((iaddr & 0xFFFFF000) != 0){
        iaddr &= 0xFFFFF000;
        iaddr += 0x1000;
    }

    if(iaddr == h-> end){
        return;
    }

    ASSERT(iaddr <= h->max);

    // assume our previous end address is page aligned
    ASSERT((h->end & 0xFFFFF000) == 0);
    uint32_t i = 0;
    for(i=iaddr;i<h->end;i-=0x1000){
        free_frame(get_page(i, 1, curr_dir));
    }

    h->end = iaddr;
}

int do_brk(void* addr){
    uint32_t pid = 0;
    heap_t* curr;
    
    if(pid == 0){
        if(kheap!=NULL) curr = kheap;
    }

    if(addr>curr->end){
        expand(curr, addr); 
    } else if(addr<curr->end){
        contract(curr, addr);
    }
}

uintptr_t do_getbrk(){
    //todo: actually request the pid
    uint32_t pid = 0;
    
    if(pid == 0){
        if(kheap!=NULL) return kheap->end;
    }

    return 0;
}
