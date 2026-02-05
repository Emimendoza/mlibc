#pragma once
#include <stddef.h>
#include <_mlibc_internal/macros.h>
#include <_mlibc_internal/cpp_start.h>

void* memcpy(void* __MLIBC_RESTRICT dest, const void* __MLIBC_RESTRICT src, size_t n);
void* memmove(void* __MLIBC_RESTRICT dest, const void* __MLIBC_RESTRICT src, size_t n);
void* memset(void* s, int c, size_t n);


#include <_mlibc_internal/cpp_end.h>