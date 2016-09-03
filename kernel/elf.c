#include <kernel/elf.h>
#include <kernel/kheap.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/** 
 * A parser for the ELF executable file format. The parse_elf function reads 
 * the file header and the program and section headers. Accessing the program 
 * and section header tables are done differently than expected because of 
 * their structure in the memory (it took me an unusually long time for me figure
 * that out). It then currently loops over the headers and just displays some 
 * information(seemingly correct/matches objdump). In the future it won't need
 * to do that anymore
**/

int parse_elf(fs_node_t* file, elf_header_t* header, elf_ph_t* pht, elf_sh_t* sht){
    /* get the ELF header, its format is documented in the header file
     * (it's the same as the format of the struct) */
    header = (elf_header_t*)kmalloc(sizeof(elf_header_t));
    memset(header, 0, sizeof(elf_header_t));

    file->read(file, 0, sizeof(elf_header_t), header);

    /* get the sizes of the program and section header tables
     * and allocate memory for those sizes */

    pht = (elf_ph_t*)kmalloc(header->phentsize*header->phnum);
    sht = (elf_sh_t*)kmalloc(header->shentsize*header->shnum);

    file->read(file, header->phoff, header->phentsize*header->phnum, pht);
    file->read(file, header->shoff, header->shentsize*header->shnum, sht);

    /* control the magic number to ensure we have a valid ELF header
     we need to control the cpu architecture in the future as well */
    ASSERT(header->magic==ELF_MAGIC);

    // assume we have only one program header
    printf("t:%x o:%x v:%x p:%x f:%x m:%x flags:%x a:%x \n", pht[0].type, pht[0].offset,
           pht[0].vaddr, pht[0].paddr, pht[0].filesz, pht[0].memsz,
           pht[0].flags, pht[0].align);

    /* fetch the names into a string buffer. Shouldn't be more than a few hundred
     * bytes long */
    char* namesbuf = kmalloc(sht[header->shstrndx].size);
    file->read(file, sht[header->shstrndx].offset, sht[header->shstrndx].size, namesbuf);

    uint32_t i;
    for(i=0;i<header->shnum;i++){
        printf("%s %x %x %x %x %x \n", (namesbuf+sht[i].name), sht[i].type, sht[i].flags, sht[i].address, sht[i].offset, sht[i].size);
    }

    return 0; // success
}
