#pragma once
#include "internals/bits/wordsize.h"
#include "internals/bits/int.h"
#include "internals/bits/uint.h"


#if _MLIBC_WORDSIZE == 8
typedef uint64_t size_t;
typedef int64_t ptrdiff_t;
#elif _MLIBC_WORDSIZE == 4
typedef uint32_t size_t;
typedef int32_t ptrdiff_t;
#endif