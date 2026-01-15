#include <mlibc/entry.h>
#include <mlibc/macros.h>
#include <linux/auxvec.h>
#include <assert.h>
#include <mlibc/syscall.h>
#include <sys/syscall.h>

extern "C" {
extern int main(...);
extern void_fn_t __init_array_start[];
extern void_fn_t __init_array_end[];
extern void_fn_t __fini_array_start[];
extern void_fn_t __fini_array_end[];
}

struct info_from_kernel{
	int argc;
	char* argv0;
};



auxv_t *mlibc_auxv;

void __mlibc_init(void* stack_pointer) {
	auto* info = static_cast<info_from_kernel*>(stack_pointer);
	const auto envp_start = &info->argv0 + info->argc + 1;
	auto envp = envp_start;
	while (*envp != nullptr) envp++;
	mlibc_auxv = reinterpret_cast<auxv_t*>(++envp);
	// Call global constructors
	for (auto fn = __init_array_start; fn != __init_array_end; ++fn) {
		(*fn)();
	}
	const int main_ret = main(info->argc, &info->argv0, envp_start);
	// Call global destructors
	for (auto fn = __fini_array_start; fn != __fini_array_end; ++fn) {
		(*fn)();
	}
	sys(__NR_exit, main_ret);
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
