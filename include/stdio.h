#pragma once
#include "internals/bits/internal-unsigned.h"

typedef struct{

}mbstate_t;

typedef struct{
    mbstate_t _state;
    _mlibc_size_t _cnt;
}fpos_t;

// No one in their right mind would dare
// use the internals of this struct
typedef struct {
    char* _file_name;
    fpos_t _offset;
    char *buf;
    _mlibc_size_t buf_size;
    int fd;
    _mlibc_uint32_t _flags;
}FILE;

