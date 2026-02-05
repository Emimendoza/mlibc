#include <signal.h>
#include <mlibc/macros.h>
#include <mlibc/variables.h>
#include <mlibc/syscall.h>
#include <errno.h>

EXPORT
int raise(const int sig) {
	return sys(SYS_tgkill, mlibc::pid, mlibc::tid, sig);
}

EXPORT
int tgkill(const int tgid, const int tid, const int sig) {
	const auto ret = sys(SYS_tgkill, tgid, tid, sig);
	if (ret == 0) return ret;
	errno = -ret;
	return -1;
}