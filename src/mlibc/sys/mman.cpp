#include <sys/syscall.h>
#include <sys/mman.h>
#include <mlibc/syscall.h>
#include <mlibc/macros.h>

#include "errno.h"


EXPORT
void* mmap(void* addr, const size_t length, const int prot, const int flags, const int fd, const off_t offset) {
	const auto ret = sys(__NR_mmap, addr, length, prot, flags, fd, offset);
	if (ret < 0) {
		errno = -ret;
		return MAP_FAILED;
	}
	return reinterpret_cast<void*>(ret);
}

EXPORT
int munmap(void* addr, const size_t length) {
	const auto ret = sys(__NR_munmap, addr, length);
	if (ret < 0) {
		errno = -ret;
		return -1;
	}
	return 0;
}

