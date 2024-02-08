#pragma once
#include "memory.h"
#include <stddef.h>
#ifndef DEFAULT_VECTOR_CAPACITY
#define DEFAULT_VECTOR_CAPACITY 10
#endif

#define vector(type) \
    struct {         \
        type* data; \
        size_t size; \
        size_t capacity;\
        size_t elem_size;\
    }

#define vector_init(vec) \
    do {                 \
        (vec).elem_size = sizeof(typeof(*vec.data)); \
        (vec).data = (typeof(vec.data)) malloc(DEFAULT_VECTOR_CAPACITY*(vec).elem_size); \
        (vec).size = 0;  \
        (vec).capacity = DEFAULT_VECTOR_CAPACITY;                      \
    } while(0)

#define vector_push(vec, elem) \
    do {                        \
        if((vec).size == (vec).capacity) { \
            (vec).capacity *= 2; \
            (vec).data = (typeof(vec.data)) realloc((vec).data, (vec).capacity*(vec).elem_size); \
        } \
        (vec).data[(vec).size++] = elem; \
    } while(0)

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

#define vector_get(vec, index) \
    ((vec).data[index])

#define vector_free(vec) \
    do {                  \
        free((vec).data); \
    } while(0)
