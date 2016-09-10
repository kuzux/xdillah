#include <kernel/serial.h>

/** 
 * This file defines helper functions to use the serial port(read/write to it), 
 * and a routine to initialize a serial port. The serial port is currently only 
 * used for debugging, as it is connected to qemu's stdout, but might come in handy 
 * in the future. 
**/

// http://wiki.osdev.org/Serial_ports was super useful

void serial_init(int port){
   outb(port + 1, 0x00);    // Disable all interrupts
   outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(port + 1, 0x00);    //                  (hi byte)
   outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received(int port) {
   return inb(port + 5) & 1;
}

uint8_t read_serial(int port){
    while (serial_received(port) == 0);
 
    return inb(port);
}

int is_transmit_empty(int port) {
   return inb(port + 5) & 0x20;
}

void write_serial(uint8_t chr, int port){
    while (is_transmit_empty(port) == 0);
 
    outb(port, chr); 
}

void puts_serial(char* str, int port){
    while(*str){
        write_serial(*str, port);
        str++;
    }
}
