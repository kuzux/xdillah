#include <kernel/ioprim.h>

/**
 * Various io primitives (inb, outb) these are just simple wrappers around
 * simple assembly instructions, it is probably for the best if we move them
 * to the arch/ subdirectory. The functions are mainly used to communicate with
 * external io devices lik keyboard, serial port, or ata devices.
**/

void outb(uint16_t port, uint8_t value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void outw(uint16_t port, uint16_t value){
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

uint16_t inw(uint16_t port){
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
