#include <mlibc/entry.h>
#include <mlibc/macros.h>
#include <mlibc/syscall.h>
#include <sys/syscall.h>

extern "C" {
int main(...);
}


void __mlibc_init(void* stack_pointer) {
	sys(__NR_exit, main());
}
