#include <kernel/tasking.h>
#include <kernel/paging.h>
#include <kernel/kheap.h>
#include <kernel/timer.h>
#include <stdio.h>

//currently running task
volatile task_t* currtask;

// tasks waiting to be switched
volatile task_t* ready_q;

// those are defined in paging.c
extern void alloc_frame(page_t*,int,int);
extern uint32_t read_eip();
extern void _taskswitch(uint32_t, uint32_t, uint32_t, uint32_t);
extern void _usermode();

uint32_t nextpid = 1;
uint32_t prev_switch_time = 0;

void tasking_init(){
    currtask = ready_q = (task_t*)kmalloc(sizeof(task_t));

    // initialize the kernel task
    currtask->pid = nextpid++;
    currtask->esp = currtask->ebp = 0;
    currtask->eip = 0;
    currtask->pagedir = curr_dir; // defined in paging.h
    currtask->next = 0;
}

void taskswitch(){
    // not initialized tasking yet
    if(!currtask){ 
        return;
    }

    static int tscount = 0;
    tscount++;

    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();

    // check if we recently did a task switch
    if(prev_switch_time) {
        int newtime = timer_get_tick();
        if(newtime==prev_switch_time) { 
            return;
        } else {
            prev_switch_time = newtime;
        }
    }

    task_t* newtask = currtask->next;

    // restart at the beginning if we followed a null link
    if(!newtask) {
        newtask = ready_q;
    }

    if(tscount<10)
        printf("curr %d %x %x %x \n", currtask->pid, eip, esp, ebp);

    // save the previous values
    currtask->eip = eip;
    currtask->esp = esp;
    currtask->ebp = ebp;

    // perform the actual task switching
    currtask = newtask;

    curr_dir = currtask->pagedir;

    eip = currtask->eip;
    esp = currtask->esp;
    ebp = currtask->ebp;

    if(tscount<10)
        printf("new %d %x %x %x \n", currtask->pid, eip, esp, ebp);

    _taskswitch(eip, curr_dir->tables_phys, esp, ebp);
}

int do_fork(){
    asm volatile("cli");

    task_t* parent = currtask;

    // defined in paging.h
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
        // seemingly we get into that if but can't return
        //printf("%s \n", "forked");

        // so, here's an oddity; shouldn't both of these lines do literally the exact same thing?
        // the c version pagefaults and the asm version resets the emulator

        // is the return call pagefaulting due to the stack changing after the jump?
        // then why does the 'ret' instruction behave differently?
        // we probably need to copy the entire stack on fork

        return 0;

        asm("movl $0x0, %eax; ret");
    }
}

int do_getpid(){
    return currtask->pid;
}

void usermode(){
    // todo: deal with the kernel stack
    _usermode();
}