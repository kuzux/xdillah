#ifndef _TASKING_H
#define _TASKING_H

#include <stdint.h>
#include <kernel/paging.h>

typedef struct task {
    int pid; 
    uint32_t esp, ebp;
    uint32_t eip;
    page_directory_t* pagedir;
    struct task* next; // linked list
} task_t;

void tasking_init();
void taskswitch();

int do_fork();
int do_getpid();

#endif
