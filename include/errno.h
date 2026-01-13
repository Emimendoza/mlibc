#pragma once
#include "_mlibc_internal/cpp_start.h"

extern int *__errno_loc();
#define errno (*__errno())

#include "_mlibc_internal/cpp_end.h"