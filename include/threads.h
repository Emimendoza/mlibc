#pragma once

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
    uint32_t* futex;
    int type;
}mtx_t;

int mtx_init(mtx_t* mtx, int type);