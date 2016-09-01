#include <kernel/kheap.h>
#include <stdlib.h>
#include <stdio.h>

// curr_dir (used in expand/contract) is defined at paging.h
heap_t* kheap;

// end is defined in the linker script.
extern uint32_t __end;
uint32_t placement_address = (uint32_t)&__end;
heap_t* kheap = NULL;

void expand(heap_t* h, void* newaddr){
    uint32_t iaddr = (uint32_t)newaddr;

    ASSERT(h != NULL);
    ASSERT(iaddr > h->end);

    printf("%x \n", iaddr);

    // page align the new break
    if((iaddr & 0xFFFFF000) != 0){
        iaddr &= 0xFFFFF000;
        iaddr += 0x1000;
    }

    ASSERT(iaddr <= h->max);

    // assume our previous end address is page aligned
    ASSERT(h->end % 0x1000 == 0);

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

    if(iaddr >= h-> end){
        return;
    }

    ASSERT(iaddr <= h->max);

    // assume our previous end address is page aligned
    ASSERT(h->end % 0x1000 == 0);
    uint32_t i = 0;

    for(i=iaddr;i<h->end;i+=0x1000){
 
        free_frame(get_page(i, 1, curr_dir));
    }

    h->end = iaddr;
}


heap_t* make_heap(uint32_t start, uint32_t end, uint32_t max, 
                  uint8_t supervisor, uint8_t readonly){
    heap_t* heap = (heap_t*)kmalloc(sizeof(heap_t));

    // assume the addreses are page-aligned
    ASSERT(start%0x1000 == 0);
    ASSERT(end%0x1000 == 0);

    heap->start = start;
    heap->brk = start;
    heap->end = end;
    heap->max = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    return heap;
}

int do_brk(void* addr){
    //todo: actually request the pid
    uint32_t pid = 0;
    heap_t* curr;
    
    if(pid == 0){
        if(kheap!=NULL) curr = kheap;
    }

    uint32_t res = curr->brk;

    curr->brk = addr;

    if(curr->brk>curr->end){
        expand(curr, addr); 
    } else if(curr->brk<curr->end && curr->end-curr->brk < HEAP_MIN_SIZE){
        // todo: this function panics due to a failed assert or something
        contract(curr, addr);
    }

    return res;
}

uintptr_t do_getbrk(){
    //todo: actually request the pid
    uint32_t pid = 0;
    
    if(pid == 0){
        if(kheap!=NULL) return kheap->brk;
    }

    return 0;
}

uint32_t kmalloc_bootstrap(uint32_t sz, int align, uint32_t *phys){
    if (align && (placement_address & 0xFFFFF000)) {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc_a(uint32_t sz) {
    return kmalloc_bootstrap(sz, 1, NULL);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
    return kmalloc_bootstrap(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
    return kmalloc_bootstrap(sz, 1, phys);
}

void* kmalloc(size_t sz) {
    //if(kheap!=NULL){
        // printf("asd\n");
        // return (void*)(kmalloc_bootstrap(sz, 0, 0));        
        //return malloc(sz);
    //} else {
        // printf("qwe\n");
        return (void*)(kmalloc_bootstrap(sz, 0, 0));
    //}
}

void kfree(void* ptr) {
    if(kheap!=NULL){
        free(ptr);
    }
}