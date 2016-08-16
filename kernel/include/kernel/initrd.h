#ifndef _INITRD_H
#define _INITRD_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/multiboot.h>
#include <kernel/fs.h>

#define INITRD_MAGIC 0xDEAD4242;

typedef struct initrd_header{
    uint32_t magic;
    uint32_t numfiles;
} initrd_header_t;

typedef struct initrd_file_header{
    uint32_t magic;
    uint32_t offset;
    uint32_t size;
    uint32_t namelen;
    char name[80];
} initrd_file_header_t;

fs_node_t* initrd_parse(mb_module_t*);

#endif
