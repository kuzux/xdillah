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
#include <stdio.h>
#include <assert.h>

void kearly(){
    gdt_init();
    idt_init();
    tty_init();
    serial_init(COM1);
}

void kmain(multiboot_info_t *mbd){    
    ASSERT(mbd->mods_count > 0);

    mb_module_t* mod = (mb_module_t*)(mbd->mods_addr);

    char buf[1024];
    int size = mod->mod_end - mod->mod_start;
    memcpy(buf, (void*)mod->mod_start, size);
    buf[size] = '\0';

    printf("Module size: %d contents: %s\n", size, buf);

    void* a = kmalloc(8);
    paging_init(mbd->mem_upper*1024);
    void* b = kmalloc(8);
    void* c = kmalloc(8);
    
    asm volatile("sti");

    timer_init(50);
    kb_init();

    char* str1 = "ello vorld";
    char* str2 = "page test";

    printf("%s\n%s\n", str1, str2);

    printf("kmalloc test a:%x b:%x c:%x\n", a, b, c);

    //uint32_t *ptr = (uint32_t*)0xA0000000;
    //uint32_t do_page_fault = *ptr;

    //printf("%x\n", do_page_fault);
    puts_serial("serial test\n", COM1);
 
    for(;;);
    
    return;
}
