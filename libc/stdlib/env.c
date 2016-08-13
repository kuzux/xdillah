#include <stdio.h>
#include <stdlib.h>
 
__attribute__((__noreturn__))
void abort(void)
{
    // TODO: Add proper kernel panic.
    printf("Kernel Panic: abort()\n");
    while ( 1 ) { }
    __builtin_unreachable();
}

int atexit(void (*func)(void)){

}

int at_quick_exit(void (*func)(void)){

}

void exit(int status){

}

_Noreturn void quick_exit(int status){

}

void _Exit(int status){

}

char* getenv(const char* name){

}

int system(const char* command){
    
}
