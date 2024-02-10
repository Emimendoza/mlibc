#pragma once
#include "bits/wordsize.h"
#include "bits/uint.h"
// This header is used to define syscall numbers
// This assumes modern linux syscall numbers
#if _MLIBC_WORDSIZE == 8
enum Syscall {
    SYS_read = 0,
    SYS_write = 1,
    SYS_open = 2,
    SYS_close = 3,
    SYS_mmap = 9,
    SYS_mprotect = 10,
    SYS_munmap = 11,
    SYS_exit = 60
};
#elif _MLIBC_WORDSIZE == 4
enum Syscall {
    SYS_read = 3,
    SYS_write = 4,
    SYS_open = 5,
    SYS_close = 6,
    SYS_mmap = 90,
    SYS_mprotect = 125,
    SYS_munmap = 91,
    SYS_exit = 1
};
#else
#error "Unsupported word size"
#endif