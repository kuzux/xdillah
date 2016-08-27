#include <kernel/elf.h>
#include <kernel/kheap.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int parse_elf(fs_node_t* file, elf_header_t* header, elf_ph_t** pht, elf_sh_t** sht){
    header = (elf_header_t*)kmalloc(sizeof(elf_header_t));
    memset(header, 0, sizeof(elf_header_t));

    file->read(file, 0, sizeof(elf_header_t), header);

    pht = (elf_ph_t**)kmalloc(header->phentsize*header->phnum);
    sht = (elf_sh_t**)kmalloc(header->shentsize*header->shnum);

    memset(pht, 0, header->phentsize*header->phnum);
    memset(sht, 0, header->shentsize*header->shnum);

    //printf("%d %d \n", header->phoff, header->shoff);
    file->read(file, header->phoff, header->phentsize*header->phnum, pht);
    file->read(file, header->shoff, header->shentsize*header->shnum, sht);

    // printf("%x\n", header->magic);
    ASSERT(header->magic==ELF_MAGIC);

    // printf("%d %d %d %x \n", header->type, header->machine, header->version_32bit, header->entry);

    //printf("%d %d %d %d %d \n", header->ehsize, header->phentsize, header->phnum, header->shentsize, header->shnum);

    int i;
    for(i=0;i<header->phnum;i++){
        printf("t:%d o:%x v:%x p:%x f:%x m:%x flags:%x a:%x \n", pht[i]->type, pht[i]->offset,
               pht[i]->vaddr, pht[i]->paddr, pht[i]->filesz, pht[i]->memsz,
               pht[i]->flags, pht[i]->align);
    }

    return 0; // success
}
