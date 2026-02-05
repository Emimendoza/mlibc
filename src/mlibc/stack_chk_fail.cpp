#include <mlibc/macros.h>
#include <mlibc/syscall.h>
#include <unistd.h>
#include <stdlib.h>

static constexpr auto stack_error_msg = "Stack smashing detected\n";

CEXPORT [[noreturn]]
void __stack_chk_fail() {
	sys(SYS_write, STDERR_FILENO, stack_error_msg, sizeof(stack_error_msg) - 1);
	_Exit(127);
}