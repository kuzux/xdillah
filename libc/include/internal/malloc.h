#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct block {
    size_t size;
    struct block* next;
    struct block* prev;
    int free;
    int magic;
} block_t;

#define BLOCK_SIZE sizeof(block_t)
#define BLOCK_MAGIC 0x12345678

#endif