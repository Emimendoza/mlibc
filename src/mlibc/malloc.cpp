#include <stdlib.h>
#include <unistd.h>
#include <mlibc/syscall.h>
#include <mlibc/macros.h>
#include <stdint.h>

const uint32_t page_size = getpagesize();

EXPORT
void* malloc(size_t size) {
}

EXPORT
void free(void* ptr) {
}