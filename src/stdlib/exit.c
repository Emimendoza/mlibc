#include <stdlib.h>
#include <stdatomic.h>
#include <internals/vector.h>

#if _MLIBC_WORDSIZE == 8
[[noreturn]] extern void _mlibc_sysexit_64(int exit_code);
#define _mlibc_sysexit _mlibc_sysexit_64
#elif _MLIBC_WORDSIZE == 4
[[noreturn]] extern void _mlibc_sysexit_32(int exit_code);
#define _mlibc_sysexit _mlibc_sysexit_32
#endif
typedef void (*atexit_func)(void);

volatile vector(atexit_func) _mlibc_atexit_funcs;
volatile uint8_t _atexit_lock = 0;

[[noreturn]] void exit(int exit_code) {
    while (atomic_exchange(&_atexit_lock, 1) != 0);

    if (!vector_exits(_mlibc_atexit_funcs)) {
        atomic_store(&_atexit_lock, 0);
        _mlibc_sysexit(exit_code); // This should only happen if vector_init failed
    }

    while(!vector_empty(_mlibc_atexit_funcs)){
        vector_top(_mlibc_atexit_funcs)();
        vector_pop(_mlibc_atexit_funcs);
    }
    vector_free(_mlibc_atexit_funcs);
    atomic_store(&_atexit_lock, 0);
    _mlibc_sysexit(exit_code);
}

int atexit(void (*function)(void)) {
    while(atomic_exchange(&_atexit_lock, 1) != 0);
    if(!vector_exits(_mlibc_atexit_funcs)){
        atomic_store(&_atexit_lock, 0);
        // This should only happen if atexit is called after exit
        return 1;
    }
    int ret_val = vector_push(_mlibc_atexit_funcs, function);
    atomic_store(&_atexit_lock, 0);
    return ret_val;
}


