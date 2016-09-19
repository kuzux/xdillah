#include <kernel/tasking.h>
#include <kernel/paging.h>
#include <kernel/kheap.h>
#include <stdio.h>

//currently running task
volatile task_t* currtask;

// tasks waiting to be switched
volatile task_t* ready_q;

// those are defined in paging.c
extern void alloc_frame(page_t*,int,int);
extern uint32_t read_eip();
extern void _taskswitch(uint32_t, uint32_t);
extern void _usermode();

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
    // not initialized tasking yet
    if(!currtask){ 
        return;
    }

    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();

    // just performed a task switch
    if(eip == 0x12345){
        return;
    }

    // save the previous values
    currtask->eip = eip;
    currtask->esp = esp;
    currtask->ebp = ebp;

    // perform the actual task switching
    currtask = currtask->next;

    // restart at the beginning if we followed a null link
    if(!currtask){
        currtask = ready_q;
    }

    curr_dir = currtask->pagedir;

    eip = currtask->eip;
    esp = currtask->esp;
    ebp = currtask->ebp;

    _taskswitch(eip, curr_dir->tables_phys);
}

int do_fork(){
    asm volatile("cli");

    task_t* parent = currtask; 

    page_directory_t* dir = clone_directory(curr_dir);

    // initialize the new task
    task_t *newtask = (task_t*)kmalloc(sizeof(task_t));
    newtask->pid = nextpid++;
    newtask->esp = newtask->ebp = 0;
    newtask->eip = 0;
    newtask->pagedir = dir;
    newtask->next = 0;

    // add to the end of ready queue
    task_t* tmp = (task_t*)ready_q;
    ASSERT(tmp!=NULL);
    while(tmp->next){
        tmp = tmp->next;
    }
    tmp->next = newtask;

    uint32_t eip = read_eip();

    if(currtask==parent){
        // We are the parent, so set up the esp/ebp/eip for our child.
        
        uint32_t esp, ebp; 
        asm volatile("mov %%esp, %0" : "=r"(esp));
        asm volatile("mov %%ebp, %0" : "=r"(ebp));
        
        newtask->esp = esp;
        newtask->ebp = ebp;
        newtask->eip = eip;

        asm volatile("sti");
    } else {
        return 0;
    }
}

int do_getpid(){
    return currtask->pid;
}

void usermode(){
    // todo: deal with the kernel stack
    _usermode();
}