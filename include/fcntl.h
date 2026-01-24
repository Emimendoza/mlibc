#pragma once
#include <asm/fcntl.h>
#include <sys/types.h>
#include "_mlibc_internal/cpp_start.h"

int open(const char* pathname, int flags, mode_t mode);

#include "_mlibc_internal/cpp_end.h"