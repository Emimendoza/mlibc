#pragma once
#include "bits/wordsize.h"
#include "bits/internal-unsigned.h"
// This header is used to define syscall numbers
// This assumes modern linux syscall numbers
#if _MLIBC_WORDSIZE == 8
const uint64_t SYS_read = 0;
const uint64_t SYS_write = 1;
const uint64_t SYS_open = 2;
const uint64_t SYS_close = 3;
const uint64_t SYS_mmap = 9;
const uint64_t SYS_mprotect = 10;
const uint64_t SYS_munmap = 11;
const uint64_t SYS_exit = 60;
#elif _MLIBC_WORDSIZE == 4
const uint32_t SYS_exit = 1;
const uint32_t SYS_read = 3;
const uint32_t SYS_write = 4;
const uint32_t SYS_open = 5;
const uint32_t SYS_mmap = 90;
const uint32_t SYS_munmap = 91;
const uint32_t SYS_mprotect = 125;
#else
#error "Unsupported word size"
#endif