#ifndef _KB_H
#define _KB_H

#include <stddef.h>
#include <stdint.h>

struct kb_status {
    uint8_t shift;
    uint8_t ctrl;
    uint8_t alt;
    uint8_t capslock;
};

typedef struct kb_status kb_status_t;

void kb_init();
kb_status_t getkbstat();

#endif