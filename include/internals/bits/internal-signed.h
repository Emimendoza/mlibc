#pragma once
#include "wordsize.h"

typedef signed char _mlibc_int8_t;
typedef signed short int _mlibc_int16_t;
typedef signed int _mlibc_int32_t;
#if _MLIBC_WORDSIZE == 8
typedef signed long int _mlibc_int64_t;
typedef _mlibc_int64_t _mlibc_ptrdiff_t;
#else
__extension__ typedef signed long long int _mlibc_int64_t;
typedef _mlibc_int32_t _mlibc_ptrdiff_t;
#endif

typedef _mlibc_ptrdiff_t _mlibc_off_t;