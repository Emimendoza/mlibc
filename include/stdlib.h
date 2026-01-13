#pragma once
#include "stddef.h"
#include "_mlibc_internal/cpp_start.h"

void*	malloc(size_t size);
void*	calloc(size_t nmemb, size_t size);
void*	realloc(void* ptr, size_t size);
void	free(void* ptr);

#include "_mlibc_internal/cpp_end.h"