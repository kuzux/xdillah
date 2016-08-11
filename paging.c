#include <paging.h>
#include <bitmap.h>
#include <tty.h>
#include <kheap.h>
#include <string.h>
#include <panic.h>
#include <isr.h>
#include <kprintf.h>

uint32_t *frames;
uint32_t nframes;

// defined in kheap.c
extern uint32_t placement_address;

page_directory_t *curr_dir;
page_directory_t *kernel_dir;

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

    if(alloc == (uint32_t)-1) panic("No more frames!");
    
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

    uint32_t acc = 0;
    page_t* page;

    while(acc < placement_address){
        page = get_page(acc, 1, kernel_dir);
        alloc_frame(page, 1, 1);

        acc += 0x1000;
    }

    register_interrupt_handler(0xe, page_fault);

    switch_page_directory(kernel_dir);
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
    int id      = regs.errcode & 0x10;    // Caused by an instruction fetch?

    // Output an error message.
    kprintf("Page fault! ( ");
    if (present){ kprintf("present "); }
    if (rw){ kprintf("read-only "); }
    if (us){ kprintf("user-mode "); }
    if (reserved){ kprintf("reserved "); }
    kprintf(") at %x\n", faulting_address);

    panic("page fault");

}
