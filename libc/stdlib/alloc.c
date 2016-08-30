#include <stdlib.h>
#include <internal/malloc.h>
#include <unistd.h>

// Mostly copied & pasted from https://danluu.com/malloc-tutorial/

void* calloc(size_t num, size_t size){
    void* m = malloc(num*size);
    memset(m, 0, num*size);
    return m;
}

void* global_base = NULL;

block_t* request_space(block_t* last, size_t size){
    block_t* block;
    block = sbrk(0);
    void* request = sbrk(size+BLOCK_SIZE);

    assert(((void*)block)==request);
    if(request < 0){
        return NULL; // sbrk failed
    }

    if(last){
        last->next = block;
    }

    block->size = size;
    block->next = NULL;
    block->free=0;
    block->magic = BLOCK_MAGIC;

    return block;
}

block_t* find_block(block_t** last, size_t size){
    block_t* curr = global_base;
    while(curr && !(curr->free && curr->size >= size)){
        *last = curr;
        curr = curr->next;
    }
}

block_t* get_block(void* ptr){
    return (block_t*)(ptr-1);
}

void* malloc(size_t size){
    block_t* block;
    // todo: align size to 8 bytes (or 4K)

    if(size <= 0){
        return NULL;
    }

    if(!global_base){
        block = request_space(NULL, size);
        if(!block){
            return NULL;
        }
        global_base = block;
    } else {
        block_t* last = global_base;
        block = find_block(&last, size);
        if(!block){
            block = request_space(last, size);
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
