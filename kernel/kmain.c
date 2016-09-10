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
#include <stdlib.h>
#include <assert.h>

/** This file simply contains the kmain function, which prints a header,
 * initializes a bunch of stuff, then runs d\some test routines. Really simple to 
 * understand. 
**/

void print_header(){
    printf("Booting xdillah version %s \n", XDILLAH_VERSION);
    printf("Compiled at %s on %s \n", XDILLAH_COMPILE_DATE, XDILLAH_COMPILE_ENV);
    printf("Written by %s \n", XDILLAH_AUTHOR);
    printf("%s \n", "################################################");
}

void kearly(){
    gdt_init();
    idt_init();
    tty_init();
    serial_init(COM1);
}

extern uint32_t placement_address;

void kmain(multiboot_info_t *mbd){
    //don't use that after we init paging

    int modcount = mbd->mods_count;

    uint32_t initrd_start, initrd_end;

    if(modcount > 0){
        mb_module_t* mod = (mb_module_t*)(mbd->mods_addr);

        initrd_start = *(uint32_t*)(mbd->mods_addr);
        initrd_end = *(uint32_t*)(mbd->mods_addr+4);
        placement_address = initrd_end;
    }

    asm volatile("sti");

    print_header();

    timer_init(50);

    printf("%s \n", "timer done");

    kb_init();

    printf("%s \n", "initializing paging");

    paging_init(mbd->mem_upper*1024);

    printf("%s \n", "paging done");
    
    if(modcount > 0){
        fs_root = initrd_parse(initrd_start);
        printf("%s \n", "initrd done");
    } else {
        printf("%s \n", "fatal error: no initrd");
        abort(__FILE__, __LINE__);
    }

/*  char test[11];
    fs_node_t* test_txt = fs_root->finddir(fs_root, "initrd_contents/test.txt");
    test_txt->read(test_txt, 0, 10, test);
    test[10] = '\0';
    printf("%s \n", test);
*/

    fs_node_t* elf_test = fs_root->finddir(fs_root, "initrd_contents/elf_test");

    elf_header_t* header;
    elf_ph_t* pht;
    elf_sh_t* sht;
    parse_elf(elf_test, header, pht, sht);

    for(;;);
    
    return;
}
