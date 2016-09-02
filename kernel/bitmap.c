#include <kernel/bitmap.h>

/**
 * Utility class for a bitset. A bitset is implemented as a
 * uint32_t array, the functions in this file are used to 
 * access/modify the bitsets (easier than manually using the bit shift operations.). 
 * The compiler will hopefully inline the calls to these functions. 
 * Currently only being used to track which physical page frames 
 * are allocated to a memory page
**/

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
