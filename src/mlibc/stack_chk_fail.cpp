#include <mlibc/macros.h>
#include <assert.h>
CEXPORT [[noreturn]]
void __stack_chk_fail() {
	assert(false);
}