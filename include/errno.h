#pragma once
#include <asm/errno.h>
#include "_mlibc_internal/cpp_start.h"

extern int *__errno_loc();
#define errno (*__errno_loc())


#include "_mlibc_internal/cpp_end.h"