#pragma once
#include "_mlibc_internal/macros.h"
#include "_mlibc_internal/cpp_start.h"
#ifndef NDEBUG
__M_LIBC_NORETURN
void __mlibc_assert_fail(const char* expr, const char* file, int line);

#define assert(...) \
	do { \
		if (!(__VA_ARGS__)) { \
			__mlibc_assert_fail(#__VA_ARGS__, __FILE__, __LINE__); \
		} \
	} while (0)
#else
#define assert(...) ((void)0)
#endif


#include "_mlibc_internal/cpp_end.h"