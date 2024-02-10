#pragma once
#include "wordsize.h"

typedef unsigned char _mlibc_uint8_t;
typedef unsigned short int _mlibc_uint16_t;
typedef unsigned int _mlibc_uint32_t;
#if _MLIBC_WORDSIZE == 8
typedef unsigned long int _mlibc_uint64_t;
typedef _mlibc_uint64_t _mlibc_size_t;
#else
__extension__ typedef unsigned long long int _mlibc_uint64_t;
typedef _mlibc_uint32_t _mlibc_size_t;
#endif