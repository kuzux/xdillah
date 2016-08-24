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
#include <stdio.h>
#include <assert.h>

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

    fs_root = initrd_parse(initrd_start);
    asm volatile("sti");

    timer_init(50);
    kb_init();

    paging_init(mbd->mem_upper*1024);

    // some testing for initrd
    int i;
    for(i=0;;i++){
        struct dirent* dir = fs_root->readdir(fs_root, i);
        if(dir == NULL) break;
        printf("/%s \n", dir->name);
    }

    for(;;);
    
    return;
}
