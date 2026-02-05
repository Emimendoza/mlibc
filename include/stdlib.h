#pragma once
#include "stddef.h"
#include "_mlibc_internal/macros.h"
#include "_mlibc_internal/cpp_start.h"

// Exit codes
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


// Memory management
void*	malloc(size_t size);
void*	aligned_alloc(size_t alignment, size_t size);
void*	calloc(size_t nmemb, size_t size);
void*	realloc(void* ptr, size_t size);
void	free(void* ptr);

// Program termination

__MLIBC_NORETURN void abort() __MLIBC_NOEXCEPT;
__MLIBC_NORETURN void exit(int exit_code);
__MLIBC_NORETURN void _Exit(int exit_code) __MLIBC_NOEXCEPT;
__MLIBC_NORETURN void quick_exit(int exit_code);

// Register exit functions

int atexit(void (*func)()) __MLIBC_NOEXCEPT;
int at_quick_exit(void (*func)()) __MLIBC_NOEXCEPT;

// String stuff
unsigned long strtoul(const char* __MLIBC_RESTRICT str, char** __MLIBC_RESTRICT str_end, int base);


#include "_mlibc_internal/cpp_end.h"