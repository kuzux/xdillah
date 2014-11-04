#ifndef _BITMAP_H
#define _BITMAP_H

#include <stddef.h>
#include <stdint.h>

void bitmap_set(uint32_t* map, uint32_t bit);
void bitmap_clr(uint32_t* map, uint32_t bit);
int bitmap_test(uint32_t* map, uint32_t bit);

#endif
