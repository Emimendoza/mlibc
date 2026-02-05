#pragma once
#include "_mlibc_internal/macros.h"
#include "_mlibc_internal/cpp_start.h"

#define CHAR_BIT 8
#define SCHAR_MIN	(__MLIBC_MIN_S(signed char))
#define SCHAR_MAX	(__MLIBC_MAX_S(signed char))
#define UCHAR_MAX	(__MLIBC_MAX_U(unsigned char))
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX
#define MB_LEN_MAX	16
#define SHRT_MIN	(__MLIBC_MIN_S(short))
#define SHRT_MAX	(__MLIBC_MAX_S(short))
#define USHRT_MAX	(__MLIBC_MAX_U(unsigned short))
#define INT_MIN		(__MLIBC_MIN_S(int))
#define INT_MAX		(__MLIBC_MAX_S(int))
#define UINT_MAX	(__MLIBC_MAX_U(unsigned int))
#define LONG_MIN	(__MLIBC_MIN_S(long))
#define LONG_MAX	(__MLIBC_MAX_S(long))
#define ULONG_MAX	(__MLIBC_MAX_U(unsigned long))
#define LLONG_MIN	(__MLIBC_MIN_S(long long))
#define LLONG_MAX	(__MLIBC_MAX_S(long long))
#define ULLONG_MAX	(__MLIBC_MAX_U(unsigned long long))

#include "_mlibc_internal/cpp_end.h"