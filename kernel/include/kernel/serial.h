#ifndef _SERIAL_H
#define _SERIAL_H

#include <kernel/ioprim.h>
#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

void serial_init(int port);
uint8_t read_serial(int port);
void write_serial(uint8_t chr, int port);
void puts_serial(char* str, int port);

#endif
