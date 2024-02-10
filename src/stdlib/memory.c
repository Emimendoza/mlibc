#include <internals/memory.h>
#include <stdatomic.h>
#include <internals/mman.h>

typedef struct heap_block{
    size_t size;
    struct heap_block* next;
    struct heap_block* prev;
}heap_block;

volatile heap_block* heap = NULL;
volatile uint8_t heap_lock = 0;

void lock_heap(){
    while(atomic_exchange(&heap_lock, 1) != 0);
}
void unlock_heap(){
    atomic_store(&heap_lock, 0);
}

void* malloc(size_t size){
    if(size == 0){
        return NULL;
    }
    lock_heap();

    size_t page_size = __getpagesize();

    unlock_heap();
    return NULL;
}

void free(void* ptr){
    if(ptr == NULL){
        return;
    }
    lock_heap();

    unlock_heap();
}

void* calloc(size_t num, size_t size){
    if(num == 0 || size == 0){
        return NULL;
    }
    lock_heap();

    unlock_heap();
    return NULL;
}

void* realloc(void* ptr, size_t size){
    if(ptr == NULL){
        return malloc(size);
    }
    if(size == 0){
        free(ptr);
        return NULL;
    }
    lock_heap();

    unlock_heap();
    return NULL;
}




