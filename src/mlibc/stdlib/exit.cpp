#include <cstdlib>
#include <mlibc/macros.h>
#include <mlibc/syscall.h>
#include <signal.h>

EXPORT
void _Exit(const int exit_code)  noexcept {
	sys(SYS_exit, exit_code);
	UNREACHABLE();
}

EXPORT
void abort() noexcept{
	raise(SIGABRT);
	// Should be unnecessary but just in case raise() returns.
	_Exit(-1);
}

EXPORT
int atexit(void (*func)()) noexcept {
	// TODO: Implement function registration
	return 0;
}

EXPORT int at_quick_exit(void (*func)()) noexcept {
	// TODO: Implement function registration
	return 0;
}

EXPORT
void exit(const int exit_code) {
	// TODO: Call functions registered with atexit()
	_Exit(exit_code);
}

EXPORT
void quick_exit(const int exit_code) {
	// TODO: Call functions registered with at_quick_exit()
	_Exit(exit_code);
}