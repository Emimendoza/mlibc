#pragma once
#include "_mlibc_internal/cpp_start.h"

long syscall(long number, ...);
int getpagesize();
int close(int fd);

#include "_mlibc_internal/cpp_end.h"