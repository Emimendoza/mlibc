#include <errno.h>
#include <mlibc/macros.h>

static thread_local int thread_errno = 0;

CEXPORT
int *__errno_loc() {
	return &thread_errno;
}