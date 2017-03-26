#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/timer.h>
#include <kernel/kb.h>
#include <kernel/multiboot.h>
#include <kernel/paging.h>
#include <kernel/serial.h>
#include <kernel/kheap.h>
#include <kernel/fs.h>
#include <kernel/initrd.h>
#include <kernel/elf.h>
#include <kernel/config.h>
#include <kernel/tasking.h>
#include <kernel/amfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/** This file simply contains the kmain function, which prints a header,
 * initializes a bunch of stuff, then runs d\some test routines. Really simple to 
 * understand. 
**/


void print_header(){
    printf("Booting xdillah version %s \n", XDILLAH_VERSION);
    printf("Compiled at %s on %s \n", XDILLAH_COMPILE_DATE, XDILLAH_COMPILE_ENV);
    printf("Written by %s \n", XDILLAH_AUTHOR);
    printf("%s \n", "################################################");
}

void kearly(){
    gdt_init();
    idt_init();
    tty_init();
    serial_init(COM1);
}

extern uint32_t placement_address;

void stack_init(uint32_t init_esp, void* start, uint32_t sz){
    // the start address needs to be page aligned
    ASSERT((uint32_t)start%0x1000 == 0); 

    uint32_t i;
    for(i=(uint32_t)start;i>=(uint32_t)start-sz;i-=0x1000){
        alloc_frame(get_page(i,1,curr_dir), 1, 1);
    }

    // Flush the TLB by reading and writing the page directory address again.
    uint32_t pd_addr;
    asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
    asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

    // Old ESP and EBP, read from registers.
    uint32_t old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
    uint32_t old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

    uint32_t offset = (uint32_t)start - init_esp;

    uint32_t new_stack_pointer = old_stack_pointer + offset;
    uint32_t new_base_pointer  = old_base_pointer + offset;

    printf("%x %x %x \n", old_stack_pointer, old_base_pointer, offset);
    
    memcpy(start, (void*)old_stack_pointer, init_esp-old_stack_pointer);

    for(i = (uint32_t)start; i > (uint32_t)start-sz; i -= 4){
        uint32_t tmp = *(uint32_t*)i;
        // If the value of tmp is inside the range of the old stack, assume it is a base pointer
        // and remap it. This will unfortunately remap ANY value in this range, whether they are
        // base pointers or not.
        
        // don't move those values
        if(tmp == new_stack_pointer || tmp == new_base_pointer) continue;

        if ((old_stack_pointer < tmp) && (tmp < init_esp)){
            tmp = tmp + offset;
            uint32_t *tmp2 = (uint32_t*)i;
            *tmp2 = tmp;
        }
    }

    printf("%x %x \n", new_stack_pointer, new_base_pointer);
    
    // Change to the new stack.
    asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
    asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void kmain(multiboot_info_t *mbd, uint32_t init_stack){
    //don't use that after we init paging

    int modcount = mbd->mods_count;

    uint32_t initrd_start, initrd_end;

    if(modcount > 0){
        mb_module_t* mod = (mb_module_t*)(mbd->mods_addr);

        initrd_start = *(uint32_t*)(mbd->mods_addr);
        initrd_end = *(uint32_t*)(mbd->mods_addr+4);
        placement_address = initrd_end;
    }

    asm volatile("sti");

    print_header();

    timer_init(50);

    printf("%s \n", "timer done");

    kb_init();

    printf("%s \n", "initializing paging");

    paging_init(mbd->mem_upper*1024);

    printf("%s \n", "paging done");
    
    // that doesn't work atm.
    // osdev.org also thinks that it's pretty bad
    //stack_init(init_stack, KERNEL_STACK_START, KERNEL_STACK_SIZE);

    //printf("%s \n", "stack done");
    
    tasking_init();
    printf("%s \n", "tasking done");

    if(modcount > 0){
        fs_root = initrd_parse(initrd_start);
        printf("%s \n", "initrd done");
    } else {
        printf("%s \n", "fatal error: no initrd");
        abort(__FILE__, __LINE__);
    }
    
    fs_node_t* elf_test = fs_root->finddir(fs_root, "initrd_contents/elf_test");

    elf_header_t* header;
    elf_ph_t* pht;
    elf_sh_t* sht;
    parse_elf(elf_test, header, pht, sht);
    printf("%s \n", "elf test done");

    // multitasking test
    int res = do_fork();

    /*if(res!=0){
        printf("%s \n", "parent");
    } else {
        printf("%s \n", "child");
    }*/

    for(;;);
    
    return;
}
