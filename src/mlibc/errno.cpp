#include <errno.h>

static thread_local int thread_errno = 0;
int *__errno_loc() {
	return &thread_errno;
}