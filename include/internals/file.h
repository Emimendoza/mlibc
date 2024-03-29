#pragma once
#include <stddef.h>
#include <stdio.h>
#include "vector.h"

typedef enum{
    O_RDONLY = 0,
    O_WRONLY = 1,
    O_RDWR = 2,
    O_CREAT = 0x40,
    O_EXCL = 0x80,
    O_TRUNC = 0x200,
    O_APPEND = 0x400,
    O_NONBLOCK = 0x800,
    O_SYNC = 0x1000,
    O_DSYNC = 0x4000,
    O_RSYNC = 0x8000,
    O_DIRECTORY = 0x10000,
    O_NOFOLLOW = 0x20000,
    O_CLOEXEC = 0x40000,
    O_DIRECT = 0x80000,
    O_NOATIME = 0x100000,
    O_PATH = 0x200000,
    O_TMPFILE = 0x400000,
    O_NDELAY = O_NONBLOCK
}flags_t;

enum{
    SEEK_SET = 0,
    SEEK_CUR = 1,
    SEEK_END = 2
};



int __init_files();

int __open(const char* path, flags_t, int mode);
int __close(int fd);
ssize_t __read(int fd, void* buf, size_t count);
ssize_t __write(int fd, const void* buf, size_t count);
_mlibc_off_t __lseek(int fd, _mlibc_off_t offset, int whence);
