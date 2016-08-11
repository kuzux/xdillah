#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tty.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <kb.h>
#include <multiboot.h>
#include <paging.h>
#include <kprintf.h>

void kmain(multiboot_info_t *mbd){
    char* str1 = "ello vorld";
    char* str2 = "page test";

    gdt_init();
    idt_init();
    tty_init();
    paging_init(mbd->mem_upper*1024);
    asm volatile("sti");

    timer_init(50);
    kb_init();

    kprintf("%s\n%s\n", str1, str2);

    uint32_t *ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;

    kprintf("%x\n", do_page_fault);

    // asm volatile("int $0xe");
 
    for(;;);
    
    return;
}
