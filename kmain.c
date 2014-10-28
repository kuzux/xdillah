#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <tty.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <kb.h>

void kmain(void){
    char* str = "ello vorld\nbeya\n";

    tty_init();
    gdt_init();
    idt_init();
    asm volatile("sti");

    timer_init(50);
    kb_init();

    tty_puts(str);

    for(;;);
    
    return;
}
