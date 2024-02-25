#pragma once
#include "internals/bits/internal-signed.h"
#include "internals/bits/internal-unsigned.h"

typedef _mlibc_size_t thrd_t;

enum{
    mtx_plain = 0b1,
    mtx_recursive = 0b10,
    mtx_timed = 0b100
};

enum{
    thrd_success = 0,
    thrd_busy = 1,
    thrd_error = 2,
    thrd_nomem = 3,
    thrd_timedout = 4
};

// Using futex from linux kernel
typedef struct{
    volatile thrd_t _owner;
    volatile _mlibc_ptrdiff_t _count;
    volatile _mlibc_uint32_t _futex;
    volatile int _type;
    volatile _mlibc_int8_t _is_locked;
}mtx_t;

int mtx_init(mtx_t* mtx, int type);
int mtx_lock(mtx_t* mtx);