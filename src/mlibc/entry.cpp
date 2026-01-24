#include <mlibc/entry.h>
#include <mlibc/macros.h>
#include <linux/auxvec.h>
#include <assert.h>
#include <mlibc/syscall.h>
#include <sys/syscall.h>
#include <mlibc/variables.h>

extern "C" {
extern int main(...);
extern WEAK void (*__preinit_array_start[])();
extern WEAK void (*__preinit_array_end[])();
extern WEAK void (*__init_array_start[])();
extern WEAK void (*__init_array_end[])();
extern WEAK void (*__fini_array_start[])();
extern WEAK void (*__fini_array_end[])();
}

void call_init_funcs();
void call_fini_funcs();

struct info_from_kernel{
	int argc;
	char* argv[];
};

char** mlibc::environ = nullptr;

bool mlibc_init_called = false;
bool dynamically_linked = false;

auxv_t *mlibc_auxv;

void __mlibc_init(void* stack_pointer) {
	mlibc_init_called = true;
	auto* info = static_cast<info_from_kernel*>(stack_pointer);
	// The auxiliary vector is located after the environment pointers.
	mlibc::environ = info->argv + info->argc + 1;
	auto env_iter = mlibc::environ;
	while (*env_iter != nullptr) {
		++env_iter;
	}
	mlibc_auxv = reinterpret_cast<auxv_t*>(env_iter + 1);
	const auto page_size_entry = getauxvect(AT_PAGESZ);
	assert(page_size_entry && "AT_PAGESZ not found in auxv");
	mlibc::page_size = static_cast<uint32_t>(page_size_entry->a_un.a_val);
	call_init_funcs();
	const int main_ret = main(info->argc, info->argv, mlibc::environ);
	call_fini_funcs();
	sys(__NR_exit, main_ret);
	UNREACHABLE();
}
__attribute__((constructor))
void call_init_funcs() {
	if (!mlibc_init_called) {
		dynamically_linked = true;
		return;
	}
	if (dynamically_linked) {
		// TODO: Get __init_array_* since the dynamic linker doesn't provide it
		return;
	}
	static bool guard = false;
	if (guard) {
		return;
	}
	guard = true;
	auto test1 = __preinit_array_start;
	auto test2 = __preinit_array_end;
	auto test3 = __init_array_start;
	auto test4 = __init_array_end;

	for (auto fn = __preinit_array_start; fn < __preinit_array_end; fn++) {
		(*fn)();
	}

	for (auto fn = __init_array_start; fn < __init_array_end; fn++) {
		(*fn)();
	}
}
void call_fini_funcs() {
	for (auto fn = __fini_array_start; fn < __fini_array_end; ++fn) {
		(*fn)();
	}
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


