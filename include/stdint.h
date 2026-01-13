#pragma once
#include "_mlibc_internal/config.h"
#include "_mlibc_internal/cpp_start.h"

#ifdef __MLIBC_64
typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed int			int32_t;
typedef signed long long	int64_t;

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;
#else
#error "NOT IMPLEMENTED"
#endif
#include "_mlibc_internal/cpp_end.h"