#include <kernel/tasking.h>
#include <kernel/paging.h>
#include <kernel/kheap.h>

//currently running task
volatile task_t* currtask;

// tasks waiting to be switched
volatile task_t* ready_q;

// those are defined in paging.c
extern void alloc_frame(page_t*,int,int);
extern uint32_t read_eip();

uint32_t nextpid = 1;

void tasking_init(){
    currtask = ready_q = (task_t*)kmalloc(sizeof(task_t));

    // initialize the kernel task
    currtask->pid = nextpid++;
    currtask->esp = currtask->ebp = 0;
    currtask->eip = 0;
    currtask->pagedir = curr_dir;
    currtask->next = 0;
}

void taskswitch(){

}

int do_fork(){

}

int do_getpid(){

}
