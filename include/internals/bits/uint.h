#pragma once
#include "wordsize.h"

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
#if _MLIBC_WORDSIZE == 8
typedef unsigned long int uint64_t;
#else
__extension__ typedef unsigned long long int uint64_t;
#endif