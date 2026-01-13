#pragma once
#include "_mlibc_internal/cpp_start.h"

typedef typeof(sizeof(0))					size_t;
typedef typeof((char*)nullptr - (char*)1)	ptrdiff_t;
typedef ptrdiff_t							ssize_t;
typedef typeof(nullptr)						nullptr_t;
#define NULL nullptr
#define offsetof(type, member) ((size_t)&(((type *)nullptr)->member))

// For avx512 alignment
typedef struct alignas(64){char _;}		max_align_t;

#include "_mlibc_internal/cpp_end.h"