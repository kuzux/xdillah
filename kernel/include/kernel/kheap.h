#ifndef _BRK_H
#define _BRK_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/paging.h>
#include <assert.h>
#include <stdio.h>

#define KHEAP_START        0xC0000000
#define KHEAP_MAX          0xCFFFF000
#define HEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE    0x20000
#define HEAP_MIN_SIZE      0x70000

typedef struct {
    uint32_t start;
    uint32_t brk;
    uint32_t end;
    uint32_t max;
    uint8_t supervisor;
    uint8_t readonly;
} heap_t;

extern uint32_t placement_address;
extern heap_t* kheap;

heap_t* make_heap(uint32_t start, uint32_t end, uint32_t max, 
                  uint8_t supervisor, uint8_t readonly);
int do_brk(void* addr);

uintptr_t do_getbrk();

void* kmalloc(size_t sz);
uint32_t kmalloc_a(size_t sz);
uint32_t kmalloc_p(size_t sz, uint32_t *phys);
uint32_t kmalloc_ap(size_t sz, uint32_t *phys);
void kfree(void* ptr);

#endif
