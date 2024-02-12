#include <stdlib.h>
#include <internals/debug.h>
#define alloc_size 1024

int main(){
    size_t initial_allocations = _mlibc_alloc_counts();
    uint8_t* ptr = (uint8_t*) malloc(alloc_size);
    if(ptr == NULL){
        free(ptr);
        return 1;
    }
    if(_mlibc_alloc_counts() != initial_allocations + 1){
        free(ptr);
        return 1;
    }
    for(size_t i = 0; i < alloc_size; i++){
        ptr[i] = i%256;
    }
    for(size_t i = 0; i < alloc_size; i++){
        if(ptr[i] != i%256){
            free(ptr);
            return 1;
        }
    }
    free(ptr);
    if(_mlibc_alloc_counts() != initial_allocations){
        return 1;
    }
    uint8_t* empty = (uint8_t*) calloc(alloc_size, 1);
    if(empty == NULL){
        free(empty);
        return 1;
    }
    if(_mlibc_alloc_counts() != initial_allocations + 1){
        free(empty);
        return 1;
    }
    for(size_t i = 0; i < alloc_size; i++){
        if(empty[i] != 0){
            free(empty);
            return 1;
        }
    }
    uint8_t *n_ptr = (uint8_t*) realloc(empty, alloc_size*2);
    if(n_ptr == NULL){
        free(empty);
        free(n_ptr);
        return 1;
    }
    if(_mlibc_alloc_counts() != initial_allocations + 1){
        free(n_ptr);
        return 1;
    }
    for(size_t i = alloc_size; i < alloc_size*2; i++){
        n_ptr[i] = i%256;
    }
    for(size_t i = 0; i < alloc_size*2; i++){
        if(i<alloc_size && n_ptr[i] != 0){
            free(n_ptr);
            return 1;
        }
        if(i>=alloc_size && n_ptr[i] != i%256){
            free(n_ptr);
            return 1;
        }
    }
    int* int_ptr = (int*) malloc(alloc_size);
    if(int_ptr == NULL){
        free(int_ptr);
        return 1;
    }
    if(_mlibc_alloc_counts() != initial_allocations + 2){
        free(int_ptr);
        return 1;
    }
    free(int_ptr);
    if(_mlibc_alloc_counts() != initial_allocations + 1){
        return 1;
    }
    free(n_ptr);
    if(_mlibc_alloc_counts() != initial_allocations){
        return 1;
    }
    return 0;
}