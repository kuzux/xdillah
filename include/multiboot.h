#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#include <stddef.h>
#include <stdint.h>

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
} multiboot_info_t;

#endif
