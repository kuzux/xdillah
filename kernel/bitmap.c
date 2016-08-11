#include <kernel/bitmap.h>

#define BIT_OFFSET(x) (x%32)
#define BIT_INDEX(x) (x/32)

void bitmap_set(uint32_t* map, uint32_t bit){
    uint32_t mask = (1 << BIT_OFFSET(bit));
    map[BIT_INDEX(bit)] |= mask;
}

void bitmap_clr(uint32_t* map, uint32_t bit){
    uint32_t mask = ~(1 << BIT_OFFSET(bit));
    map[BIT_INDEX(bit)] &= mask;
}

int bitmap_test(uint32_t* map, uint32_t bit){
    uint32_t mask = (1 << BIT_OFFSET(bit));
    return map[BIT_INDEX(bit)] & mask;
}
