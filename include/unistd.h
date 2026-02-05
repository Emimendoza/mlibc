#pragma once
#include "_mlibc_internal/cpp_start.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

long syscall(long number, ...);
int getpagesize();
int close(int fd);

#include "_mlibc_internal/cpp_end.h"