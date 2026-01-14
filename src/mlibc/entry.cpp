#include <mlibc/entry.h>
#include <mlibc/macros.h>
#include <linux/auxvec.h>
#include <assert.h>
#include <mlibc/syscall.h>
#include <sys/syscall.h>

extern "C" {
int main(...);
}

struct info_from_kernel{
	int argc;
	char* argv0;
};

auxv_t *mlibc_auxv;

void __mlibc_init(void* stack_pointer) {
	auto* info = static_cast<info_from_kernel*>(stack_pointer);
	auto envp = &info->argv0 + info->argc + 1;
	while (*envp != nullptr) envp++;
	mlibc_auxv = reinterpret_cast<auxv_t*>(++envp);
	sys(__NR_exit, main());
}

const auxv_t* getauxvect(const size_t type) {
	assert(mlibc_auxv != nullptr && "getauxval called before __mlibc_init");
	for (const auxv_t* a = mlibc_auxv; a->a_type != AT_NULL; ++a) {
		if (a->a_type == type) {
			return a;
		}
	}
	return nullptr;
}
