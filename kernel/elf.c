#include <kernel/elf.h>
#include <kernel/kheap.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
/** 
 * A parser for the ELF executable file format. The parse_elf function reads 
 * the file header and the program and section headers. The parser currently 
 * doesn't seem to work correctly but, in the future, will simply populate the
 * data structures to be used in the functions responsive for executing/dynamically
 * linking the programs in ELF format. Currently, it seems like the program 
 * parses the initial header bits correctly and mangles the phoff/shoff bits.
 * I might need to work more with hex dumps of the executable files/general 
 * memory dumps at the time of the reading in the future so that I can solve
 * this issue. Work on fixing this part is currently on hold for a few days.
**/

int parse_elf(fs_node_t* file, elf_header_t* header, elf_ph_t** pht, elf_sh_t** sht){
    /* get the ELF header, its format is documented in the header file
     (it's the same as the format of the struct) */

    header = (elf_header_t*)kmalloc(sizeof(elf_header_t));
    memset(header, 0, sizeof(elf_header_t));

    file->read(file, 0, sizeof(elf_header_t), header);

    /* get the sizes of the program and section header tables
     and allocate memory for those sizes */

    pht = (elf_ph_t**)kmalloc(header->phentsize*header->phnum);
    sht = (elf_sh_t**)kmalloc(header->shentsize*header->shnum);

    memset(pht, 0, header->phentsize*header->phnum);
    memset(sht, 0, header->shentsize*header->shnum);

    //printf("%d %d \n", header->phoff, header->shoff);
    file->read(file, header->phoff, header->phentsize*header->phnum, pht);
    file->read(file, header->shoff, header->shentsize*header->shnum, sht);

    /* control the magic number to ensure we have a valid ELF header
     we need to control the cpu architecture in the future as well */
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
