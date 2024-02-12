#pragma once
#include "internals/bits/int.h"
#include "internals/bits/uint.h"

typedef _mlibc_size_t size_t;
typedef _mlibc_ptrdiff_t ptrdiff_t;
typedef _mlibc_ptrdiff_t ssize_t;
// nullptr is a C23 feature
// gcc 13 started supporting it

#if (__STDC_VERSION__ >= 201112L || __cplusplus >= 201103L) && __GNUC__ >= 13 // TODO: add clang version
#define NULL nullptr
#else
#define NULL ((void*)0)
#endif