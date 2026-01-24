#include <sys/auxv.h>
#include <errno.h>
#include <mlibc/entry.h>

unsigned long getauxval(unsigned long type) {
	auto *auxv_entry = getauxvect(type);
	if (!auxv_entry) {
		errno = ENOENT;
		return 0;
	}
	return auxv_entry->a_un.a_val;
}