#pragma once
#include <stddef.h>

// Needs to be a multiple of the page size
typedef size_t off_t;

typedef enum{
    PROT_NONE = 0,
    PROT_READ = 1,
    PROT_WRITE = 2,
    PROT_EXEC = 4
}PROT;

typedef enum {
    MAP_SHARED = 1,
    MAP_PRIVATE = 2,
    MAP_ANONYMOUS = 0x20,
    MAP_FIXED = 0x10
}MAP;


void* __mmap(void* addr, size_t length, PROT prot, MAP flags, int fd, off_t offset);
int __munmap(void* addr, size_t length);
int __mprotect(void* addr, size_t length, PROT prot);
size_t __getpagesize();
