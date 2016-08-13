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
#include <stdio.h>

void kearly(){
    gdt_init();
    idt_init();
    tty_init();
    serial_init(COM1);
}

void kmain(multiboot_info_t *mbd){
    paging_init(mbd->mem_upper*1024);
    asm volatile("sti");

    timer_init(50);
    kb_init();

    char* str1 = "ello vorld";
    char* str2 = "page test";

    printf("%s\n%s\n", str1, str2);

    //uint32_t *ptr = (uint32_t*)0xA0000000;
    //uint32_t do_page_fault = *ptr;

    //printf("%x\n", do_page_fault);
    puts_serial("serial test\n", COM1);
 
    for(;;);
    
    return;
}
