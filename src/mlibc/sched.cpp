#include <sched.h>
#include <mlibc/syscall.h>

EXPORT
int sched_yield() {
	sys(SYS_sched_yield);
	return 0;
}