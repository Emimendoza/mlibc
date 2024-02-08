#include <stdlib.h>
#include <threads.h>
#include <internals/vector.h>

#if _MLIBC_WORDSIZE == 8
[[noreturn]] extern void _mlibc_sysexit_64(int exit_code);
#define _mlibc_sysexit _mlibc_sysexit_64
#elif _MLIBC_WORDSIZE == 4
[[noreturn]] extern void _mlibc_sysexit_32(int exit_code);
#define _mlibc_sysexit _mlibc_sysexit_32
#endif
typedef void (*atexit_func)(void);

vector(atexit_func) _mlibc_atexit_funcs;

[[noreturn]] void exit(int exit_code) {
    while(!vector_empty(_mlibc_atexit_funcs)){
        vector_top(_mlibc_atexit_funcs)();
        vector_pop(_mlibc_atexit_funcs);
    }

    _mlibc_sysexit(exit_code);
}

int atexit(void (*function)(void)) {
    return vector_push(_mlibc_atexit_funcs, function);
}


