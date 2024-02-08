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

// Returns 0 on success 1 on failure
#define vector_init(vec) ({ \
    do {                 \
        (vec).elem_size = sizeof(typeof(*vec.data)); \
        (vec).data = (typeof(vec.data)) malloc(DEFAULT_VECTOR_CAPACITY*(vec).elem_size); \
        (vec).size = 0;  \
        (vec).capacity = DEFAULT_VECTOR_CAPACITY; \
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

#define vector_get(vec, index) \
    ((vec).data[index])

#define vector_free(vec) \
    do {                  \
        free((vec).data); \
    } while(0)

// Returns 0 on success 1 on failure
#define vector_push(vec, elem) ({ \
    int result;                   \
    if((vec).size == (vec).capacity) { \
        (vec).capacity *= 2; \
        typeof(vec.data) res_ptr = (typeof(vec.data)) realloc((vec).data, (vec).capacity*(vec).elem_size); \
        if(res_ptr == NULL) { \
            result = 1; \
        } else { \
            (vec).data = res_ptr; \
            (vec).data[(vec).size] = elem; \
            (vec).size++; \
            result = 0; \
        }                         \
    } else { \
        (vec).data[(vec).size] = elem; \
        (vec).size++; \
        result = 0; \
    }                             \
    result;                       \
})
