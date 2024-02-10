#pragma once
#include "internals/bits/int.h"
#include "internals/bits/uint.h"

typedef _mlibc_size_t size_t;
typedef _mlibc_ptrdiff_t ptrdiff_t;
typedef _mlibc_ptrdiff_t ssize_t;
// nullptr is a C23 feature
// gcc 13 started supporting it

#if defined(__GCC__) && __GCC__ >= 13
#define NULL nullptr
#else
#define NULL ((void*)0)
#endif