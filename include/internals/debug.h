#pragma once
#include "bits/internal-unsigned.h"

[[noreturn]] void __stack_chk_fail();
[[noreturn]] void __failure(const char* msg);
void _mlibc_dealloc_all();
_mlibc_size_t _mlibc_alloc_counts();