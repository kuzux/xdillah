#ifndef _ELF_H
#define _ELF_H

#include <stdint.h>
#include <stddef.h>
#include <kernel/fs.h>

/* useful resources for elf:

https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
http://www.skyfree.org/linux/references/ELF_Format.pdf
http://wiki.osdev.org/ELF

*/

#define ELF_MAGIC 0x464c457f

#define ELF_32BIT 1
#define ELF_64BIT 2
#define ELF_LE 1
#define ELF_BE 2

#define ELF_RELOCATABLE 1
#define ELF_EXECUTABLE 2
#define ELF_SHARED 3
#define ELF_CORE 4

#define ELF_NOISA 0
#define ELF_SPARC 2
#define ELF_X86 3
#define ELF_MIPS 8
#define ELF_PPC 20
#define ELF_ARM 40
#define ELF_SUPERH 42
#define ELF_IA64 0x32
#define ELF_X8664 0x3E
#define ELF_AARCH64 0xB7

#define ELF_PT_NULL 0
#define ELF_PT_LOAD 1
#define ELF_PT_DYNAMIC 2
#define ELF_PT_INTERP 3
#define ELF_PT_NOTE 4
#define ELF_PT_SHLIB 5
#define ELF_PT_PHDR 6
#define ELF_PT_LOOS 0x60000000
#define ELF_PT_HIOS 0x6FFFFFFF
#define ELF_PT_LOPROC 0x70000000
#define ELF_PT_HIPROC 0x7FFFFFFF

struct elf_header {
    uint32_t magic; // 0x7f ELF
    uint8_t bits; // 1 for 32 2 for 64
    uint8_t endianness; // 1 little endian, 2 big endian
    uint8_t version;
    uint8_t osabi; // 
    uint8_t abiversion; // don't really care
    uint8_t pad[7];
    uint16_t type;
    uint16_t machine;
    uint32_t version_32bit; // set to 1 
    uint32_t entry; // points to the entry point
    uint32_t phoff; // points to program header
    uint32_t shoff; // points to the section header
    uint32_t flags; // ignored for x86
    uint16_t ehsize; // size of this header
    uint16_t phentsize; // size of an entry in the program header table
    uint16_t phnum; // number of entries in the program header table
    uint16_t shentsize; // size of an entry in the section header table
    uint16_t shnum; // number of entries in the section header table
    uint16_t shstrndx; // Contains index of the section header table entry that contains the section names.
} __attribute__((packed));

typedef struct elf_header elf_header_t;

struct elf_ph {
    uint32_t type; // type of the segment
    uint32_t offset; // offset of the segment in the file
    uint32_t vaddr; // virtual address of the segment
    uint32_t paddr; // physical address, if applicable
    uint32_t filesz; // size of the segment in bytes
    uint32_t memsz; // size of the segment in memory
    uint32_t flags; // segment-dependent
    uint32_t align; // 0 and 1 specifies no alignment. Otherwise should be a positive, integral power of 2, with p_vaddr equating p_offset modulus p_align.
} __attribute__((packed));

typedef struct elf_ph elf_ph_t;

struct elf_sh {
    uint32_t name; // offset to a string in the .shstrtab section that represents the name of this section
    uint32_t type;
    uint32_t flags;
    uint32_t address; // Virtual address of the section in memory, for sections that are loaded
    uint32_t offset; // Offset of the section in the file image
    uint32_t size; // Size in bytes of the section in the file image
} __attribute__((packed));

typedef struct elf_sh elf_sh_t;

int parse_elf(fs_node_t* file, elf_header_t* header, elf_ph_t** pht, elf_sh_t** sht);

#endif