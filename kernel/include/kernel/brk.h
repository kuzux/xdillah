#ifndef _BRK_H
#define _BRK_H

#include <stddef.h>
#include <stdint.h>

int do_brk(void* addr);
uintptr_t do_getbrk();

#endif
