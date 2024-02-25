#pragma once
#include "internals/bits/internal-unsigned.h"
#include <stdarg.h>
// 8kb buffer size
#define BUFSIZ (1024*8)

// File mode flags
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define EOF (-1)

enum {
    _MLIBC_RONLY = 0b0,
    _MLIBC_WONLY = 0b1,
    _MLIBC_WR = 0b10
};

typedef struct{

}mbstate_t;

typedef struct{
    mbstate_t _state;
    _mlibc_size_t _cnt;
}fpos_t;

// No one in their right mind would dare
// use the internals of this struct

typedef struct{
    fpos_t _offset;
    _mlibc_uint8_t* _buf;
    _mlibc_size_t _vec_index;
    _mlibc_size_t _buf_begin;
    _mlibc_size_t _buf_size;
    _mlibc_size_t _buf_cnt;
    int _fd;
    _mlibc_uint8_t _mode;
    _mlibc_uint8_t _buf_mode;
    volatile _mlibc_uint8_t _lock;
} _mlibc_FILE;

typedef _mlibc_FILE FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

FILE* fopen(const char* restrict file_name, const char* restrict mode);
int fclose(FILE* stream);
int fflush(FILE* stream);

int vfprintf(FILE* restrict stream, const char* restrict format, va_list arg);
int fprintf(FILE* restrict stream, const char* restrict format, ...);



