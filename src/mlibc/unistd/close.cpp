#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <mlibc/syscall.h>
#include <mlibc/macros.h>

EXPORT
int close(const int fd) {
	const auto ret = sys(__NR_close, fd);
	if (ret < 0) {
		errno = -ret;
		return -1;
	}
	return 0;
}