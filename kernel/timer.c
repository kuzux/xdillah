#include <kernel/timer.h>
#include <kernel/ioprim.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/isr.h>
#include <kernel/tty.h>

/**
 * This file defines functions related to the CPU clock, the helper
 * function simply reads the number of clock ticks. The initialize
 * function calculates the divisor via the given frequency and registers 
 * an interrupt handler that simply increments the tick count. 
**/

uint32_t tick = 0;

// defined in tasking.c
extern void taskswitch();

static void timer_cb(){
    tick++;
    //taskswitch();
}

void timer_init(uint32_t freq){
    if(freq <= 18){
        // divisor needs to fit in 16 bytes, which requires freq > 18
        printf("%s\n", "timer frequency set too small");
    }

    register_interrupt_handler(IRQ0, timer_cb);

    uint16_t divisor = 1193180 / freq;
    uint8_t lo = (uint8_t)(divisor & 0xFF);
    uint8_t hi = (uint8_t)((divisor >> 8) & 0xFF);

    // send the command byte
    outb(0x43, 0x36);

    // send divisor as 2 bytes
    outb(0x40, lo);
    outb(0x40, hi);
}

uint32_t timer_get_tick(){
    return tick;
}
