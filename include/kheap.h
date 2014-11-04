#ifndef _KHEAP_H
#define _KHEAP_H

#include <stddef.h>
#include <stdint.h>

uint32_t kmalloc(size_t sz);
uint32_t kmalloc_a(size_t sz);
uint32_t kmalloc_p(size_t sz, uint32_t *phys);
uint32_t kmalloc_ap(size_t sz, uint32_t *phys);

#endif