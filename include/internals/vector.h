#pragma once
#include "memory.h"
#include <stddef.h>
#include <string.h>
#ifndef DEFAULT_VECTOR_CAPACITY
#define DEFAULT_VECTOR_CAPACITY 10
#endif
#ifndef _MLIBC_EXPAND_MACRO
#define EXPAND(arg) EXPAND1(EXPAND1(EXPAND1(EXPAND1(arg))))
#define EXPAND1(arg) EXPAND2(EXPAND2(EXPAND2(EXPAND2(arg))))
#define EXPAND2(arg) EXPAND3(EXPAND3(EXPAND3(EXPAND3(arg))))
#define EXPAND3(arg) EXPAND4(EXPAND4(EXPAND4(EXPAND4(arg))))
#define EXPAND4(arg) arg
#endif

#define vector(type) \
    struct {         \
        type* data; \
        size_t size; \
        size_t capacity;\
        size_t elem_size; \
    }

int _mlibc_vector_remove_at_generic(volatile size_t* size, size_t elem_size, void* data, size_t index);

int _mlibc_vector_push_generic(volatile size_t* size, volatile size_t* capacity, size_t elem_size, void** data, void* elem);

// Returns 0 on success 1 on failure
#define vector_init(vec) ({ \
    do {                 \
        (vec).elem_size = sizeof(typeof(*vec.data)); \
        (vec).data = (typeof(vec.data)) malloc(DEFAULT_VECTOR_CAPACITY*(vec).elem_size); \
        (vec).size = 0;  \
        (vec).capacity = DEFAULT_VECTOR_CAPACITY;    \
    } while(0);  \
    (vec).data == NULL; \
})

// Assumes that the vector is not empty
#define vector_pop(vec) \
    do {                \
        if((vec).size > 0) { \
            (vec).size--; \
        } \
    } while(0)

#define vector_size(vec) \
    ((vec).size)

#define vector_top(vec) \
    ((vec).data[(vec).size-1])

#define vector_empty(vec) \
    ((vec).size == 0)

#define vector_get(vec, index) ((vec).data[index])

#define vector_exits(vec) ((vec).data != NULL)

#define vector_free(vec) \
    do {                  \
        free((vec).data); \
    } while(0)

// Returns 0 on success 1 on failure
#define vector_push(vec, elem) ({ \
    typeof(elem) _mlibc_elem_copy = elem; \
    _mlibc_vector_push_generic(&(vec).size, &(vec).capacity, (vec).elem_size, (void**)&(vec).data, &_mlibc_elem_copy);\
})


// Returns 0 on success 1 on failure
#define vector_remove_at(vec, index) \
    _mlibc_vector_remove_at_generic(&(vec).size, (vec).elem_size, (vec).data, index)
