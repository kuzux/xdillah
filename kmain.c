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

void kmain(multiboot_info_t *mbd){
    char* str = "ello vorld\nbeya\n";

    gdt_init();
    idt_init();
    tty_init();
    paging_init(mbd->mem_upper*1024);
    asm volatile("sti");

    timer_init(50);
    kb_init();

    tty_puts(str);

    uint32_t *ptr = (uint32_t*)0x00100000;
    uint32_t do_page_fault = *ptr;

    tty_putdec(do_page_fault);

    // asm volatile("int $0xd");
 
    for(;;);
    
    return;
}
