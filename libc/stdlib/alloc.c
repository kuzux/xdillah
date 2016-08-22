#include <stdlib.h>
#include <internal/malloc.h>

void* calloc(size_t num, size_t size){
    void* m = malloc(num*size);
    memset(m, 0, num*size);
    return m;
}

void* global_base = NULL;

void init_block(block_t* blk){

}

block_t request_space(block_t* last, size_t size){
    block_t block;
    // block 
    // init_block(&)
}

block_t* find_block(block_t** last, size_t size){

}

block_t* get_block(void* ptr){

}

void* malloc(size_t size){
    block_t* block;
    // todo: align size

    if(size <= 0){
        return NULL;
    }

    if(!global_base){
        // block = request_space(NULL, size);
        if(!block){
            return NULL;
        }
        global_base = block;
    } else {
        block_t* last = global_base;
        // block = find_block(&last, size);
        if(!block){
            // block = request_space(last, size);
            if(!block){
                return NULL;
            } else {
                return malloc(size);
            }
        } else {
            // todo: split blocks
            block->free = 0;
            block->magic = BLOCK_MAGIC;
        }
    }

    return (block+1);
}

void* realloc(void* ptr, size_t size){

}

void free(void* ptr){
    if(!ptr){
        return;
    }

    // todo: merge blocks
    block_t* block = get_block(ptr);
    assert(block->free==0);
    assert(block->magic==BLOCK_MAGIC);
    block->free=1;
    block->magic=BLOCK_MAGIC;
}
