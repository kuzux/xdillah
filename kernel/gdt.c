#include <kernel/gdt.h>

/** 
 * Functions related to accessing/modifying the global descriptor table
 * entries. These functions are very architecture-dependent and are mostly 
 * defined in assembly files in arch/ directory
 * For more detailed info regarding the GDT, see the GDT section on 
 * JamesM's kernel development tutorial
**/

extern void gdt_flush(uint32_t);
extern void tss_flush();

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdt_entries[num].base_low     = (base & 0xFFFF);
    gdt_entries[num].base_middle  = (base >> 16) & 0xFF;
    gdt_entries[num].base_high    = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low    = limit & 0xFFFF;
    gdt_entries[num].granularity  = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access       = access;
}

void gdt_init(){
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user mode data segment

    gdt_flush((uint32_t)&gdt_ptr);
    //tss_flush();
}
