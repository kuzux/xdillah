#ifndef _UNISTD_H
#define _UNISTD_H

#include <stdint.h>
#include <stddef.h>

int brk(void* addr);
void* sbrk(intptr_t delta);

#endif