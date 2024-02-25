#include <internals/vector.h>

int _mlibc_vector_remove_at_generic(volatile size_t* size, size_t elem_size, void* data, size_t index){
    if(*size > 0 && index < *size) {
        memmove((uint8_t*)data+index*elem_size, (uint8_t*)data+(index+1)*elem_size, (*size-index-1)*elem_size);
        (*size)--;
        return 0;
    }
    return 1;
}

int _mlibc_vector_push_generic(volatile size_t* size, volatile size_t* capacity, size_t elem_size, void** data, void* elem){
    if (*data == NULL){return 1;}
    if (*size == *capacity){
        void* new_ptr = realloc(*data, *capacity*2*elem_size);
        if(new_ptr == NULL) return 1;
        *capacity *= 2;
        *data = new_ptr;
    }
    memcpy((uint8_t*)*data+*size*elem_size, elem, elem_size);
    (*size)++;
    return 0;
}