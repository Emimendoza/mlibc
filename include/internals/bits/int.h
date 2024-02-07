#pragma once
#include "wordsize.h"

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
#if _MLIBC_WORDSIZE == 8
typedef signed long int int64_t;
#else
__extension__ typedef signed long long int int64_t;
#endif