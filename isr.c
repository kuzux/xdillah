#include <isr.h>
#include <tty.h>
#include <ioprim.h>

isr_t handlers[256];

void isr_handler(registers_t regs){
    if(handlers[regs.intno] != 0){
        isr_t handler = handlers[regs.intno];
        handler(regs);
    } else {
        tty_puts("unhandled interrupt: ");
        tty_puthex(regs.intno);
        tty_putch('\n');
    }
}

void irq_handler(registers_t regs){
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.intno >= 40){
    // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    // call the interrupt handler if set
    if (handlers[regs.intno] != 0){
        isr_t handler = handlers[regs.intno];
        handler(regs);
    }
}

void register_interrupt_handler(uint8_t idx, isr_t cb){
    handlers[idx] = cb;
}
