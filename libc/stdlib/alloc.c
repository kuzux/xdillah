#include <stdlib.h>
#include <internal/malloc.h>
#include <unistd.h>
#include <assert.h>

// Mostly copied & pasted from https://danluu.com/malloc-tutorial/
// http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf is another good resource

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
    block->prev = last;
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

void split_block(block_t* blk, size_t sz){
    if(sz + BLOCK_SIZE >= blk) return;

    block_t* newblk = (block_t*)(blk+BLOCK_SIZE+sz);
    newblk->size = blk->size - BLOCK_SIZE - sz;
    newblk->next = blk->next;
    blk->next = newblk;
    newblk->prev = blk;
    newblk->free = 1;
    newblk->magic = BLOCK_MAGIC;
}

void merge_blocks(block_t* blk){
    while(blk->free == 0 && blk->next && blk->next->free == 0){
        blk->next = blk->next->next;
        blk->size += blk->next->size + BLOCK_SIZE;
    }

    while(blk->free == 0 && blk->prev && blk->prev->free == 0){
        blk->prev->next = blk->next;
        blk->prev->size += blk->size + BLOCK_SIZE;
        blk = blk->prev;
    }
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
            block->free = 0;
            block->magic = BLOCK_MAGIC;
            split_block(block, size);
        }
    }

    return (block+1);
}

void* realloc(void* ptr, size_t size){
    if(!ptr) return malloc(size);
    block_t* block = get_block(ptr);

    if(block->size>=size) return ptr;

    void* new_ptr = malloc(size);

    if(!new_ptr) return NULL;

    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    return new_ptr;
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
    merge_blocks(block);
}
