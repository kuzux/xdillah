#include <kernel/elf.h>
#include <kernel/kheap.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int parse_elf(fs_node_t* file, elf_header_t* header, elf_ph_t** pht, elf_sh_t** sht){
    header = (elf_header_t*)kmalloc(sizeof(elf_header_t));
    pht = (elf_ph_t**)kmalloc(header->phentsize*header->phnum);
    sht = (elf_sh_t**)kmalloc(header->shentsize*header->shnum);

    memset(header, 0, sizeof(elf_header_t));

    file->read(file, 0, sizeof(elf_header_t), header);

    memset(pht, 0, header->phentsize*header->phnum);
    memset(sht, 0, header->shentsize*header->shnum);

    file->read(file, header->phoff, header->phentsize*header->phnum, pht);
    file->read(file, header->shoff, header->shentsize*header->shnum, sht);

    // printf("%x\n", header->magic);
    ASSERT(header->magic==ELF_MAGIC);

    //printf("%d %d %d %d %d \n", header->ehsize, header->phentsize, header->phnum, header->shentsize, header->shnum);

    int i;
    for(i=0;i<header->phnum;i++){
        printf("t:%d o:%d v:%d p:%d f:%d m:%d flags:%d a:%d \n", pht[i]->type, pht[i]->offset,
               pht[i]->vaddr, pht[i]->paddr, pht[i]->filesz, pht[i]->memsz,
               pht[i]->flags, pht[i]->align);
    }

    return 0; // success
}
