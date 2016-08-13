#include <stdio.h>
#include <stdlib.h>

void (*exit_funcs[255])(void);
void (*quickexit_funcs[255])(void);

uint32_t exit_funcs_id = 0;
uint32_t quickexit_funcs_id = 0;

__attribute__((__noreturn__))
void abort(void)
{
    // TODO: Add proper kernel panic.
    printf("Kernel Panic: abort()\n");
    while ( 1 ) { }
    __builtin_unreachable();
}

int atexit(void (*func)(void)){
    exit_funcs[exit_funcs_id++] = func;
}

int at_quick_exit(void (*func)(void)){
    quickexit_funcs[quickexit_funcs_id++] = func;
}

void exit(int status){
  /*
    Objects associated with the current thread with thread storage duration are destroyed (C++11 only).
    Objects with static storage duration are destroyed (C++) and functions registered with atexit are called.
    All C streams (open with functions in <cstdio>) are closed (and flushed, if buffered), and all files created with tmpfile are removed.
    Control is returned to the host environment.
  */

    uint32_t i;

    for(i=0; i<exit_funcs_id; i++)
        exit_funcs[i]();
}

_Noreturn void quick_exit(int status){
  /*
    Terminates the process normally by returning control to the host environment after calling all functions registered using at_quick_exit.

    No additional cleanup tasks are performed: No object destructors are called. Although whether C streams are closed and/or flushed, and files open with tmpfile are removed depends on the particular system or library implementation.
  */
    
    uint32_t i;

    for(i=0; i<quickexit_funcs_id; i++)
        quickexit_funcs[i]();
}

void _Exit(int status){
  /*  
    Terminates the process normally by returning control to the host environment, but without performing any of the regular cleanup tasks for terminating processes (as function exit does).

    No object destructors, nor functions registered by atexit or at_quick_exit are called.

    Whether C streams are closed and/or flushed, and files open with tmpfile are removed depends on the particular system or library implementation.
  */
}

char* getenv(const char* name){
    return NULL;
}

int system(const char* command){
    return -1;
}
