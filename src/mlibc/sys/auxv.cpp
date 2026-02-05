#include <sys/auxv.h>
#include <errno.h>
#include <mlibc/entry.h>

using namespace mlibc;
unsigned long getauxval(const unsigned long type) {
	auto *auxv_entry = getauxvect(type);
	if (!auxv_entry) {
		errno = ENOENT;
		return 0;
	}
	return auxv_entry->a_un.a_val;
}