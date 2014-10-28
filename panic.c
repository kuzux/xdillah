#include <panic.h>
#include <tty.h>

void panic(char* msg){
    tty_puts("Kernel panic: ");
    tty_puts(msg);
    tty_putch('\n');

    for(;;);
}