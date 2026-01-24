#pragma once
#include <stddef.h>
#include <sys/types.h>
#include <asm/mman.h>
#include "_mlibc_internal/cpp_start.h"

#define MAP_FAILED ((void*) -1)

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void* addr, size_t length);


#include "_mlibc_internal/cpp_end.h"