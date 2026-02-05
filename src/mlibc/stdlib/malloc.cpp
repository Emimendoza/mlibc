#include <stdlib.h>
#include <unistd.h>
#include <mlibc/syscall.h>
#include <mlibc/macros.h>
#include <stdint.h>

EXPORT
void* malloc(size_t size) {
	return nullptr;
}

EXPORT
void free(void* ptr) {
}