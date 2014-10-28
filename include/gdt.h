#ifndef _GDT_H
#define _GDT_H

#include <stddef.h>
#include <stdint.h>

/* access byte & granularity byte explanation

access byte:
===============================================================================
7 | 6 5 | 4  | 3    0
P | DPL | DT | Type

P = present (1 if segment exists)
DPL = Descriport Privilege Level, rings 0-3
DT = Descriptor type
Type = Segment type (code segment/data segment)

granularity byte:
===============================================================================
7 | 6 | 5 | 4 | 3    0
G | D | 0 | A | L

G = granularity (0 = 1 byte, 1 = 1kbyte)
D = operand size (0 = 16 bit, 1 = 32 bit)
0 = should always be 0
A = available for system use, should be left 0
L = segment length 19:16, i.e. high 4 bytes of the limit
*/

struct gdt_entry {
    uint16_t limit_low;           // The lower 16 bits of the limit.
    uint16_t base_low;            // The lower 16 bits of the base.
    uint8_t  base_middle;         // The next 8 bits of the base.
    uint8_t  access;              // Access flags, determine what ring this segment can be used in.
    uint8_t  granularity;
    uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct gdt_ptr gdt_ptr_t;

void gdt_init();

#endif
