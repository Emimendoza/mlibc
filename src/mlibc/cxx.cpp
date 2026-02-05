#include <mlibc/macros.h>
#include <sched.h>


enum state {
	UNINITIALIZED = 0,
	INITIALIZING = 1,
	INITIALIZED = 2

};

CWEAK
int __cxa_guard_acquire(volatile int* g) {
	// Using intrinsics and builtins for atomic operations
	if (__atomic_load_n(g, __ATOMIC_ACQUIRE) == INITIALIZED) {
		return 0;
	}
	// We have to block here until the guard is initialized
	while (true) {
		int expected = UNINITIALIZED;
		if (__atomic_compare_exchange_n(g, &expected, INITIALIZING, false,
						__ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {
			// We are the thread that will initialize the guard
			return 1;
		}
		if (expected == INITIALIZED) {
			// Another thread finished initializing the guard
			return 0;
		}
		sched_yield();
	}
}

CWEAK
int __cxa_guard_release(volatile int* g) {
	__atomic_store_n(g, INITIALIZED, __ATOMIC_RELEASE);
	return 0;
}

