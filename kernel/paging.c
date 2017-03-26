#include <kernel/paging.h>
#include <kernel/bitmap.h>
#include <kernel/tty.h>
#include <kernel/kheap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/isr.h>

/** This file defines a bunch of helper functions and initializers
 * regarding page tables and directories. The helpers generally 
 * deal with page frames and allocating/deallocating them and
 * the initializer basically allocates pages for the memory used by
 * the kernel. The page fault handler just prints out the location  
 * of the fault and then panics. For more detailed info see the 
 * paging section of JamesM tutorial. 
**/

// bitmap whether the page frames in the memory are empty
// or full
uint32_t *frames;
uint32_t nframes;

// called in paging_init and do_fork
page_directory_t* clone_directory(page_directory_t* src){
    uint32_t phys;

    page_directory_t* dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    memset(dir, 0, sizeof(page_directory_t));

    uint32_t offset = (uint32_t)dir->tables_phys - (uint32_t)dir;

    dir->phys_addr = phys + offset;

    uint32_t i;
    for(i=0;i<1024;i++){
        if(!src->tables[i]){
            continue;
        }

        if(kernel_dir->tables[i]==src->tables[i]){
            // we're in the kernel directory
            dir->tables[i] = src->tables[i];
            dir->tables_phys[i] = src->tables_phys[i];
        } else {
            // we're in userland, copy the table instead of linking it
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tables_phys[i] = phys | 0x07;
        }
    }

    return dir;
}

extern void copy_page_phys(void* src, void* dest);

page_table_t* clone_table(page_table_t* src, uint32_t* phys){
    page_table_t* tbl = (page_table_t*)kmalloc_ap(sizeof(page_table_t), phys);
    memset(tbl, 0, sizeof(page_table_t));

    printf("%s \n", "clonetbl");

    uint32_t i;
    for(i=0;i<1024;i++){
        if(!src->pages[i].frame){
            continue;
        }

        alloc_frame(&tbl->pages[i], 0, 0);

        tbl->pages[i].present = src->pages[i].present;
        tbl->pages[i].write = src->pages[i].write;
        tbl->pages[i].user = src->pages[i].user;
        tbl->pages[i].accessed = src->pages[i].accessed;
        tbl->pages[i].dirty = src->pages[i].dirty;

        copy_page_phys(src->pages[i].frame * 0x1000, tbl->pages[i].frame);
    }
    return tbl;
}

uint32_t next_page(){
    int i, n, lim;
    n = nframes/32;

    for(i=0;i<n;i++){
        if(frames[i] == 0xFFFFFFFF) continue;
 
        lim = i + 32;
        for(int j=i;j<lim;j++){
            if(!bitmap_test(frames, j)) return j;
        }
    }

    return (uint32_t) -1;
}

void alloc_frame(page_t* page, int kernel, int write){
    if(page->frame) return; // page already has a frame

    uint32_t alloc = next_page();

    if(alloc == (uint32_t)-1){
        //todo: hdd swaping?
        printf("%s\n", "No more frames!");
        abort(__FILE__,__LINE__);
    }
    
    bitmap_set(frames, alloc);

    page->present = 1;
    page->write   = write?1:0;
    page->user    = kernel?0:1;
    page->frame   = alloc;
}

void free_frame(page_t* page){
    if(!(page->frame)) return;

    bitmap_clr(frames, page->frame);
    page->present = 0;
    page->frame   = 0;
}

void paging_init(uint32_t memsize){
    kernel_dir = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_dir, 0, sizeof(page_directory_t));
    kernel_dir->phys_addr = (uint32_t)&(kernel_dir->tables_phys);

    nframes = memsize / 0x1000;
    frames = (uint32_t*) kmalloc(nframes/32);
    memset(frames, 0, nframes/8);

    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's
    // to be created where necessary. We can't allocate frames yet because they
    // they need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!
    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START+HEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_dir);

     uint32_t acc = 0;
     page_t* page;

    // allocate frames for already alloc'd memory and 4K more for some extra
    // we alloc memory between this and completely activating kheap
    while(acc < placement_address+0x10000){
        page = get_page(acc, 1, kernel_dir);
        alloc_frame(page, 1, 1);

        acc += 0x1000;
    }

    for (i = KHEAP_START; i < KHEAP_START+HEAP_INITIAL_SIZE; i += 0x1000){
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
    }

    register_interrupt_handler(0xe, page_fault);

    switch_page_directory(kernel_dir);

    kheap = make_heap(KHEAP_START, KHEAP_START+HEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
    curr_dir = clone_directory(kernel_dir);
    switch_page_directory(curr_dir);
}

void switch_page_directory(page_directory_t* dir){
    curr_dir = dir;

    uint32_t cr0;
    asm volatile("mov %0, %%cr3" :: "b"(&dir->tables_phys));
    asm volatile("mov %%cr0, %0" : "=b"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "b"(cr0));
}

page_t* get_page(uint32_t addr, int make, page_directory_t* dir){
    uint32_t page_id = addr / 0x1000;
    uint32_t table_id = page_id / 1024;
    uint32_t page_off = page_id % 1024;

    if(dir->tables[table_id]){
        return &dir->tables[table_id]->pages[page_off];
    } else if(make){
        uint32_t tmp;
        dir->tables[table_id] = (page_table_t*) kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_id], 0, 0x1000);

        dir->tables_phys[table_id] = tmp | 0x7;
        
        return &dir->tables[table_id]->pages[page_off];
    } else {
        return 0;
    }
}

void page_fault(registers_t regs){
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present  = !(regs.errcode & 0x1); // Page not present
    int rw       = regs.errcode & 0x2;    // Write operation?
    int us       = regs.errcode & 0x4;    // Processor was in user-mode?
    int reserved = regs.errcode & 0x8;    // Overwritten CPU-reserved bits of page entry?
    int id       = regs.errcode & 0x10;   // Caused by an instruction fetch?

    // Output an error message.
    printf("Page fault! ( ");
    if (present){ printf("present "); }
    if (rw){ printf("read-only "); }
    if (us){ printf("user-mode "); }
    if (reserved){ printf("reserved "); }
    printf(") at %x\n", faulting_address);

    abort(__FILE__, __LINE__);

}
