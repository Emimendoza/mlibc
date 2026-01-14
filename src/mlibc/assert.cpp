#include <mlibc/macros.h>
#include "assert.h"

EXPORT
void __mlibc_assert_fail(const char* expr, const char* file, int line) {
	// In a real implementation, you would log the error message or handle it appropriately.
	// For this example, we will just use an infinite loop to simulate a crash.
	while (true) {}
}