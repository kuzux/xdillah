#ifndef _BRK_H
#define _BRK_H

#include <stddef.h>
#include <stdint.h>

#define KHEAP_START        0xC0000000
#define KHEAP_MAX          0xCFFFF000
#define HEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE    0x20000
#define HEAP_MAGIC         0x123890AB
#define HEAP_MIN_SIZE      0x70000

typedef struct {
    uint32_t start;
    uint32_t end;
    uint32_t max;
    uint8_t supervisor;
    uint8_t readonly;
} heap_t;

int do_brk(void* addr);
uintptr_t do_getbrk();

#endif
