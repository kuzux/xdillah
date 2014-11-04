#ifndef _PAGING_H
#define _PAGING_H

#include <stddef.h>
#include <stdint.h>
#include <isr.h>

typedef struct page {
    uint32_t present  : 1;   // 1 if present
    uint32_t write    : 1;   // 1 if writable
    uint32_t user     : 1;   // 1 = user mode, 0 = kernel mode
    uint32_t accessed : 1;   // 1 if it's been accessed since last refresh
    uint32_t dirty    : 1;   // 1 if it's been written since last refresh
    uint32_t unused   : 7;   // ignore
    uint32_t frame    : 20;  // frame address (shifted right 12 bits)
} page_t; 

typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    // array of pointers to ptables
    page_table_t *tables[1024];

    // physical addresses of the same page tables, for loading into cr3 register
    uint32_t tables_phys[1024];

    // physical address of the tables_phys.
    uint32_t phys_addr;
} page_directory_t;

// memory size in kbytes;
void paging_init(uint32_t memsize);
void switch_page_directory(page_directory_t *dir);
// if make == 1, create the page if it's not present
page_t *get_page(uint32_t addr, int make, page_directory_t *dir);

void page_fault(registers_t regs);

#endif
