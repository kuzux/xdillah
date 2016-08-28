#ifndef _SETJMP_H
#define _SETJMP_H

#include <stdint.h>

typedef struct {
    uint8_t data[64];
} jmp_buf;

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int ret);

#endif
