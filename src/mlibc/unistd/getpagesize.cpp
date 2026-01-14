#include <unistd.h>
#include <linux/auxvec.h>
#include <mlibc/macros.h>
#include <mlibc/entry.h>
#include <assert.h>

EXPORT
int getpagesize() {
	auto* auxv_entry = getauxvect(AT_PAGESZ);
	assert(auxv_entry && "AT_PAGESZ not found in auxv");
	return static_cast<int>(auxv_entry->a_un.a_val);
}