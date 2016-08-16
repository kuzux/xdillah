#include <kernel/ordered_array.h>

int standard_lessthan_predicate(type_t a, type_t b){
    return (a<b)?1:0;
}

ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than){
    ordered_array_t ret;
    ret.array = kmalloc(max_size*sizeof(type_t));
    memset(ret.array, 0, max_size*sizeof(type_t));
    ret.size = 0;
    ret.max_size = max_size;
    ret.less_than = less_than;

    return ret;
}

ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than){
    ordered_array_t ret;
    ret.array = (type_t*)addr;
    memset(ret.array, 0, max_size*sizeof(type_t));
    ret.size = 0;
    ret.max_size = max_size;
    ret.less_than = less_than;

    return ret;
}

void destroy_ordered_array(ordered_array_t *array){
    kfree(array->array);
}

void insert_ordered_array(type_t item, ordered_array_t *array){
    ASSERT(array->less_than);
    uint32_t it = 0;
    type_t tmp, tmp2;
    
    while(it<array->size && array->less_than(array->array[it], item)) it++;

    if(it == array->size){
        array->array[array->size++] = item;
    } else {
        tmp = array->array[it];

        while(it < array->size){
            it++;
            tmp2 = array->array[it];
            array->array[it] = tmp;
            tmp = tmp2;
        }

        array->size++;
    }
}

type_t lookup_ordered_array(uint32_t i, ordered_array_t *array){
    ASSERT(i < array->size);
    return array->array[i];
}

void remove_ordered_array(uint32_t i, ordered_array_t *array){
    while(i < array->size){
        array->array[i] = array->array[i+1];
        i++;
    }

    array->size--;
}
