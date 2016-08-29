#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/timer.h>
#include <kernel/kb.h>
#include <kernel/multiboot.h>
#include <kernel/paging.h>
#include <kernel/serial.h>
#include <kernel/kheap.h>
#include <kernel/fs.h>
#include <kernel/initrd.h>
#include <kernel/elf.h>
#include <stdio.h>
#include <assert.h>

void print_header(){
    printf("Booting xdillah version %s \n", XDILLAH_VERSION);
    printf("Compiled at %s on %s \n", XDILLAH_COMPILE_DATE, XDILLAH_COMPILE_ENV);
    printf("Written by %s \n", XDILLAH_AUTHOR);
    printf("################################################ \n");
}

void kearly(){
    gdt_init();
    idt_init();
    tty_init();
    serial_init(COM1);
}

extern uint32_t placement_address;

void kmain(multiboot_info_t *mbd){
    // assume we have an initrd
    ASSERT(mbd->mods_count > 0);

    //don't use that after we init paging
    mb_module_t* mod = (mb_module_t*)(mbd->mods_addr);

    uint32_t initrd_start = *(uint32_t*)(mbd->mods_addr);
    uint32_t initrd_end = *(uint32_t*)(mbd->mods_addr+4);
    placement_address = initrd_end;

    asm volatile("sti");

    timer_init(50);
    kb_init();

    paging_init(mbd->mem_upper*1024);
    fs_root = initrd_parse(initrd_start);

    // some testing for initrd
    int i;
    for(i=0;;i++){
        struct dirent* dir = fs_root->readdir(fs_root, i);
        if(dir == NULL) break;
        printf("/%s \n", dir->name);
    }

/*  char test[11];
    fs_node_t* test_txt = fs_root->finddir(fs_root, "initrd_contents/test.txt");
    test_txt->read(test_txt, 0, 10, test);
    test[10] = '\0';
    printf("%s \n", test);
*/

    fs_node_t* elf_test = fs_root->finddir(fs_root, "initrd_contents/elf_test");

    elf_header_t* header;
    elf_ph_t** pht;
    elf_sh_t** sht;
    parse_elf(elf_test, header, pht, sht);

    print_header();

    for(;;);
    
    return;
}
