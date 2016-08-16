#include <kernel/kheap.h>

// end is defined in the linker script.
extern uint32_t __end;
uint32_t placement_address = (uint32_t)&__end;

heap_t* kheap = NULL;

static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, 
                                   heap_t *heap){
    uint32_t it = 0;
    header_t* header;
    while(it < heap->index.size){
        header = (header_t*)lookup_ordered_array(it, &heap->index);
    
        if(page_align>0){
            uint32_t location = (uint32_t)header;
            uint32_t offset = 0;

            // page align the usable address, i.e. the part after the header
            if(((location+sizeof(header_t)) & 0xFFFFF000) != 0){
                offset = 0x1000 - (location+sizeof(header_t))%0x1000;
            }

            int hole_size = header->size - offset;

            // can we fit when aligned
            if(hole_size <= (int)(header->size)) break;
        } else if(size <= header->size){
            break;
        }

        it++;
    }

    // we exited the loop, either succesfully or unsuccesfully
    if(it==heap->index.size){
        return -1;
    } else {
        return it;
    }
}

int header_less_than(void* a, void* b){
    return (((header_t*)a)->size<((header_t*)b)->size)?1:0;
}

heap_t* create_heap(uint32_t start, uint32_t end, uint32_t max, 
                    uint8_t supervisor, uint8_t readonly){
    heap_t* heap = (heap_t*)kmalloc(sizeof(heap_t));

    // assume start and end addresses are page aligned
    ASSERT(start%0x1000==0);
    ASSERT(end%0x1000==0);

    heap->index = place_ordered_array((void*)start, HEAP_INDEX_SIZE, 
                                      &header_less_than);

    // Shift the start address forward to resemble where we can start putting data.
    start += sizeof(type_t)*HEAP_INDEX_SIZE;

    // Make sure the start address is page-aligned.
    if ((start & 0xFFFFF000) != 0){
        start &= 0xFFFFF000;
        start += 0x1000;
    }

    heap->start_address = start;
    heap->end_address = end;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    // We start off with one large hole in the index.
    header_t *hole = (header_t *)start;
    hole->size = end-start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    insert_ordered_array((void*)hole, &heap->index);

    return heap;
}

static uint32_t expand(uint32_t new_size, heap_t* heap){
    ASSERT(new_size > heap->end_address - heap->start_address);

    // find the closest page boundary
    if((new_size & 0xFFFFF000) != 0){
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    ASSERT(heap->start_address + new_size <= heap->max_address);

    uint32_t i = heap->end_address - heap->start_address;

    while(i<new_size){
        alloc_frame(get_page(heap->start_address+i, 1, kernel_dir), 
                    heap->supervisor, heap->readonly);
        i += 0x1000;
    }

    heap->end_address = heap->start_address+new_size;

    return new_size;
}

static uint32_t contract(uint32_t new_size, heap_t* heap){
    ASSERT(new_size > heap->end_address - heap->start_address);

    // find the closest page boundary
    if(new_size & 0xFFFFF000 != 0){
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    ASSERT(heap->start_address + new_size <= heap->max_address);

    uint32_t i = heap->end_address - heap->start_address - 0x1000;

    while(i<new_size){
        free_frame(get_page(heap->start_address+i, 1, kernel_dir));
        i -= 0x1000;
    }

    heap->end_address = heap->start_address+new_size;

    return new_size;
}

void* alloc_heap(uint32_t size, uint8_t page_align, heap_t* heap){
    // Make sure we take the size of header/footer into account.
    uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
    // Find the smallest hole that will fit.
    int32_t it = find_smallest_hole(new_size, page_align, heap);
   
    if (it == -1){ // If we didn't find a suitable hole
        // Save some previous data.
        uint32_t old_length = heap->end_address - heap->start_address;
        uint32_t old_end_address = heap->end_address;

        // We need to allocate some more space.
        expand(old_length+new_size, heap);
        uint32_t new_length = heap->end_address-heap->start_address;

        // Find the endmost header. (Not endmost in size, but in location).
        it = 0;
        // Vars to hold the index of, and value of, the endmost header found so far.
        int idx = -1; 
        uint32_t value = 0x0;
        while (it < heap->index.size){
            uint32_t tmp = (uint32_t)lookup_ordered_array(it, &heap->index);
            if (tmp > value){
                value = tmp;
                idx = it;
            }
            it++;
        }

        // If we didn't find ANY headers, we need to add one.
        if (idx == -1){
            header_t *header = (header_t *)old_end_address;
            header->magic = HEAP_MAGIC;
            header->size = new_length - old_length;
            header->is_hole = 1;
            footer_t *footer = (footer_t *) (old_end_address + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            insert_ordered_array((void*)header, &heap->index);
        } else {
            // The last header needs adjusting.
            header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size += new_length - old_length;
            // Rewrite the footer.
            footer_t *footer = (footer_t *) ( (uint32_t)header + header->size - sizeof(footer_t) );
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        // We now have enough space. Recurse, and call the function again.
        return alloc_heap(size, page_align, heap);
    }
    header_t *orig_hole_header = (header_t *)lookup_ordered_array(it, &heap->index);
    uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size = orig_hole_header->size;

    // Here we work out if we should split the hole we found into two parts.
    // Is the original hole size - requested hole size less than the overhead for adding a new hole?
    if (orig_hole_size-new_size < sizeof(header_t)+sizeof(footer_t)){
        // Then just increase the requested size to the size of the hole we found.
        size += orig_hole_size-new_size;
        new_size = orig_hole_size;
    }
    // If we need to page-align the data, do it now and make a new hole in front of our block.
    if (page_align && orig_hole_pos&0xFFFFF000){
        uint32_t new_location   = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
        header_t *hole_header = (header_t *)orig_hole_pos;
        hole_header->size     = 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        footer_t *hole_footer = (footer_t *) ((uint32_t)new_location - sizeof(footer_t));
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    } else {
        // Else we don't need this hole any more, delete it from the index.
        remove_ordered_array(it, &heap->index);
    }

    // Overwrite the original header...
    header_t *block_header  = (header_t *)orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    // ...And the footer
    footer_t *block_footer  = (footer_t *) (orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;

    // We may need to write a new hole after the allocated block.
    // We do this only if the new hole would have positive size...
    if (orig_hole_size - new_size > 0) {
        header_t *hole_header = (header_t *) (orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;
        footer_t *hole_footer = (footer_t *) ( (uint32_t)hole_header + orig_hole_size - new_size - sizeof(footer_t) );
        if ((uint32_t)hole_footer < heap->end_address){
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        // Put the new hole in the index;
        insert_ordered_array((void*)hole_header, &heap->index);
   }


    // ...And we're done!
    return (void *)((uint32_t)block_header+sizeof(header_t));
}

void free_heap(void* ptr, heap_t* heap){
    // Exit gracefully for null pointers.
    if (ptr == 0)
        return;

    // Get the header and footer associated with this pointer.
    header_t *header = (header_t*)((uint32_t)ptr - sizeof(header_t));
    footer_t *footer = (footer_t*)((uint32_t)header + header->size - sizeof(footer_t));

    // Sanity checks.
    ASSERT(header->magic == HEAP_MAGIC);
    ASSERT(footer->magic == HEAP_MAGIC);

    // Make us a hole.
    header->is_hole = 1;

    // Do we want to add this header into the 'free holes' index?
    char do_add = 1;

    // Unify left
    // If the thing immediately to the left of us is a footer...
    footer_t *test_footer = (footer_t*) ( (uint32_t)header - sizeof(footer_t));
    if (test_footer->magic == HEAP_MAGIC &&
        test_footer->header->is_hole == 1)
    {
        uint32_t cache_size = header->size; // Cache our current size.
        header = test_footer->header;     // Rewrite our header with the new one.
        footer->header = header;          // Rewrite our footer to point to the new header.
        header->size += cache_size;       // Change the size.
        do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
    }

    // Unify right
    // If the thing immediately to the right of us is a header...
    header_t *test_header = (header_t*) ( (uint32_t)footer + sizeof(footer_t));
    if (test_header->magic == HEAP_MAGIC &&
        test_header->is_hole){
        header->size += test_header->size; // Increase our size.
        test_footer = (footer_t*) ((uint32_t)test_header + // Rewrite it's footer to point to our header.
                                   test_header->size - sizeof(footer_t) );
        footer = test_footer;
        // Find and remove this header from the index.
        uint32_t it = 0;
        while ((it < heap->index.size) &&
               (lookup_ordered_array(it, &heap->index) != (void*)test_header))
            it++;

        // Make sure we actually found the item.
        ASSERT(it < heap->index.size);
        // Remove it.
        remove_ordered_array(it, &heap->index);
    }

    // If the footer location is the end address, we can contract.
    if ((uint32_t)footer+sizeof(footer_t) == heap->end_address){
        uint32_t old_length = heap->end_address-heap->start_address;
        uint32_t new_length = contract(((uint32_t)header) - heap->start_address, heap);
        // Check how big we will be after resizing.
        if (header->size - (old_length-new_length) > 0){
            // We will still exist, so resize us.
            header->size -= old_length-new_length;
            footer = (footer_t*)(((uint32_t)header) + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            // We will no longer exist :(. Remove us from the index.
            uint32_t it = 0;
            while((it < heap->index.size) &&
                  (lookup_ordered_array(it, &heap->index) != (void*)test_header))
                it++;
            // If we didn't find ourselves, we have nothing to remove.
            if (it < heap->index.size)
                remove_ordered_array(it, &heap->index);
        }
    }

    // If required, add us to the index.
    if (do_add == 1)
        insert_ordered_array((void*)header, &heap->index);
}

uint32_t kmalloc_bootstrap(uint32_t sz, int align, uint32_t *phys){
    if (align && (placement_address & 0xFFFFF000)) {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc_a(uint32_t sz) {
    return kmalloc_bootstrap(sz, 1, NULL);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
    return kmalloc_bootstrap(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
    return kmalloc_bootstrap(sz, 1, phys);
}

void* kmalloc(size_t sz) {
    if(kheap!=NULL){
        return alloc_heap(sz, 0, kheap);
    } else {
        return (void*)(kmalloc_bootstrap(sz, 0, 0));
    }
}

void kfree(void* ptr) {
    if(kheap!=NULL){
        free_heap(ptr, kheap);
    }
}
