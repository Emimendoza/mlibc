#include <internals/memory.h>
#include <stdatomic.h>
#include <internals/mman.h>
#include <internals/debug.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    struct child_block_t* next;
    struct child_block_t* prev;
    struct heap_block_t* parent;
    size_t size;
    bool is_free;
}child_block_t;

typedef struct{
     size_t size;
     size_t free_size;
     size_t allocated_children;
     struct heap_block_t* next;
     struct heap_block_t* prev;
     child_block_t *first_child;
}heap_block_t;

volatile heap_block_t heap = {
    .size = 0,
    .free_size = 0,
    .allocated_children = 0,
    .next = NULL,
    .prev = NULL,
    .first_child = NULL
};

volatile uint8_t heap_lock = 0;

void lock_heap(){
    while(atomic_exchange(&heap_lock, 1) != 0);
}
void unlock_heap(){
    atomic_store(&heap_lock, 0);
}

size_t round_up(size_t size, size_t multiple){
    if(size % multiple == 0){
        return size;
    }
    return size + (multiple - (size % multiple));
}

 child_block_t* find_child_block( heap_block_t* parent, size_t size){
     child_block_t* block = parent->first_child;
    while(block != NULL){
        if(block->is_free && block->size >= size){
            return block;
        }
        block = ( child_block_t *) block->next;
    }
    return NULL;
}

 heap_block_t* find_block( heap_block_t* start, size_t size){
     heap_block_t* block = start;
    while(block != NULL){
        if(block->free_size >= size){
            if(find_child_block(block, size) != NULL){
                return block;
            }
        }
        block = ( heap_block_t *) block->next;
    }
    return NULL;
}

 heap_block_t* get_last_block(heap_block_t* start){
     heap_block_t* block = start;
    while(block->next != NULL){
        block = ( heap_block_t *) block->next;
    }
    return block;
}

 heap_block_t* create_block( heap_block_t* prev, size_t size){
    const MAP flags = MAP_PRIVATE | MAP_ANONYMOUS;
    const PROT prot = PROT_READ | PROT_WRITE;
    size = round_up(size, __getpagesize());
     heap_block_t* block = ( heap_block_t *) __mmap(NULL, size, prot, flags, -1, 0);
    if(block == NULL){
        return NULL;
    }
    block->size = size;
    block->free_size = size - sizeof(heap_block_t) - sizeof(child_block_t);
    block->next = NULL;
    block->prev = ( struct heap_block_t *) prev;
    prev->next = ( struct heap_block_t *) block;
    block->first_child = ( child_block_t *) (((uint8_t*)block) + sizeof(heap_block_t));
    block->first_child->size = block->free_size;
    block->first_child->is_free = true;
    block->allocated_children = 0;
    block->first_child->next = NULL;
    block->first_child->prev = NULL;
    block->first_child->parent = ( struct heap_block_t *) block;
    return block;
}

void* _malloc_no_lock(size_t size){
    if(size == 0) {
        return NULL;
    }
    // Finding first block that fits
    heap_block_t* block = find_block(&heap, size+sizeof(child_block_t));

    // If no block is found, create a new one
    if(block == NULL){
        block = create_block(get_last_block(&heap), size+sizeof(heap_block_t)+sizeof(child_block_t));
        if(block == NULL){
            unlock_heap();
            return NULL;
        }
    }

    // Finding first child block that fits
    child_block_t* child = find_child_block(block, size+sizeof(child_block_t));

    // This should never return NULL
    if(child == NULL){
        unlock_heap();
        return NULL;
    }

    // Splitting the block if there is enough space
    if (child->size > size + sizeof(child_block_t)) {
        child_block_t* new_child = ( child_block_t *) (((uint8_t*) child) + size + sizeof(child_block_t));
        new_child->size = child->size - size - sizeof(child_block_t);
        new_child->is_free = true;
        new_child->next = child->next;
        new_child->prev = ( struct child_block_t *) child;
        new_child->parent = ( struct heap_block_t *) block;
        child->size = size;
        child->next = ( struct child_block_t *) new_child;
        block->free_size -= sizeof (child_block_t);
        if (new_child->next != NULL) {
            child_block_t *next = (child_block_t *) new_child->next;
            next->prev = ( struct child_block_t *) new_child;
        }
    }
    child->is_free = false;
    block->allocated_children++;
    block->free_size -= child->size;
    return (void *) (child + 1);
}

void* malloc(size_t size){
    if(size == 0){
        return NULL;
    }
    lock_heap();
    void* ptr = _malloc_no_lock(size);
    unlock_heap();
    return ptr;
}

int optimize_block(heap_block_t* block){
    // We return the memory to the system if there are no allocated children
    if(block->allocated_children == 0){
        if(block->prev != NULL){
            heap_block_t *prev = (heap_block_t *) block->prev;
            prev->next = block->next;
        }
        if(block->next != NULL){
            heap_block_t *next = (heap_block_t *) block->next;
            next->prev = block->prev;
        }
        return __munmap((void*) block, block->size);
    }
    // Else we merge any free children
    child_block_t* child = block->first_child;
    while(child!= NULL && child->next != NULL){
        if(child->is_free && ((child_block_t*)child->next)->is_free){
            child_block_t* next = (child_block_t*)child->next;
            child->size += next->size + sizeof(child_block_t);
            child->next = next->next;
            if(next->next != NULL){
                ((child_block_t*)next->next)->prev = (struct child_block_t*)child;
            }
            block->free_size += sizeof(child_block_t);
        }
        child = (child_block_t*)child->next;
    }
    return 0;
}

void _free_no_lock(void* ptr){
    if(ptr == NULL){
        return;
    }
    child_block_t* block = (child_block_t *) ptr - 1;
    block->is_free = true;
    heap_block_t* parent_block = (heap_block_t *) block->parent;
    parent_block->free_size += block->size;
    parent_block->allocated_children--;
    if(optimize_block(parent_block)!=0){
        __failure("Failed to deallocate memory");
    }
}

void free(void* ptr){
    if(ptr == NULL){
        return;
    }
    lock_heap();
    _free_no_lock(ptr);
    unlock_heap();
}

void* calloc(size_t num, size_t size){
    if(num == 0 || size == 0){
        return NULL;
    }
    size_t bytes = num * size;
    void* ptr = malloc(bytes);
    if(ptr != NULL){
        memset(ptr, 0, bytes);
    }
    return ptr;
}

void shrink_child(child_block_t* child, size_t new_size){
    if(child->size - new_size < sizeof(child_block_t)){
        return;
    }
    child_block_t* new_child = (child_block_t*)(((uint8_t*)child) + new_size + sizeof(child_block_t));
    new_child->size = child->size - new_size - sizeof(child_block_t);
    new_child->is_free = true;
    new_child->next = child->next;
    new_child->prev = (struct child_block_t*)child;
    new_child->parent = (struct heap_block_t*)child->parent;
    child->size = new_size;
    child->next = (struct child_block_t*)new_child;
    if(new_child->next != NULL){
        ((child_block_t*)new_child->next)->prev = (struct child_block_t*)new_child;
    }
    heap_block_t* parent = (heap_block_t*)child->parent;
    parent->free_size += new_child->size + sizeof(child_block_t);
    if(optimize_block(parent)!=0){
        __failure("Failed to deallocate memory");
    }
}

// Assumes that child->next is NOT NULL and is FREE and child->next->size >= new_size
void grow_child(child_block_t* child, size_t new_size){
    child_block_t* next = (child_block_t*)child->next;
    if(next->size - new_size < sizeof(child_block_t)){
        child->size += next->size + sizeof(child_block_t);
        child->next = next->next;
        if(next->next != NULL){
            ((child_block_t*)next->next)->prev = (struct child_block_t*)child;
        }
        heap_block_t* parent = (heap_block_t*)child->parent;
        parent->free_size += sizeof(child_block_t);
        if(optimize_block(parent)!=0){
            __failure("Failed to deallocate memory");
        }
        return;
    }
    size_t diff = new_size - child->size - sizeof(child_block_t);
    child_block_t* new_child = (child_block_t*)(((uint8_t*)child) + new_size + sizeof(child_block_t));
    new_child->size = next->size - diff;
    new_child->is_free = true;
    new_child->next = next->next;
    new_child->prev = (struct child_block_t*)child;
    new_child->parent = (struct heap_block_t*)child->parent;
    child->size = new_size;
    child->next = (struct child_block_t*)new_child;
    if(new_child->next != NULL){
        ((child_block_t*)new_child->next)->prev = (struct child_block_t*)new_child;
    }
    heap_block_t* parent = (heap_block_t*)child->parent;
    parent->free_size += new_child->size + sizeof(child_block_t);
    if(optimize_block(parent)!=0){
        __failure("Failed to deallocate memory");
    }
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
    child_block_t* block = (child_block_t *) ptr - 1;
    // Find if we have to shrink or grow
    if(block->size > size){
        // Shrink
        shrink_child(block, size);
        unlock_heap();
        return ptr;
    }

    child_block_t* next = (child_block_t*)block->next;
    if(next != NULL && next->is_free && next->size >= size - block->size - sizeof(child_block_t)){
        // Grow
        grow_child(block, size);
        unlock_heap();
        return ptr;
    }
    // Otherwise, we are going to have to allocate a new block
    void* new_ptr = _malloc_no_lock(size);
    if(new_ptr == NULL){
        unlock_heap();
        return NULL;
    }
    new_ptr = memcpy(new_ptr, ptr, block->size);
    _free_no_lock(ptr);
    unlock_heap();
    return new_ptr;
}

void _mlibc_dealloc_all(){
    lock_heap();
    heap_block_t* block = &heap;
    while(block != NULL){
        heap_block_t* next = (heap_block_t*)block->next;
        __munmap((void*) block, block->size);
        block = next;
    }
    unlock_heap();
}

size_t _mlibc_alloc_counts(){
    lock_heap();
    heap_block_t* top = &heap;
    size_t count = 0;
    while(top!=NULL){
        count += top->allocated_children;
        top = (heap_block_t*)top->next;
    }
    unlock_heap();
    return count;
}



