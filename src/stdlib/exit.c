#include <stdlib.h>
#include <internals/vector.h>

[[noreturn]] extern void _mlibc_sysexit(int exit_code);
typedef void (*atexit_func)(void);

vector(atexit_func) _mlibc_atexit_funcs;

[[noreturn]] void exit(int exit_code) {
    while(!vector_empty(_mlibc_atexit_funcs)){
        vector_top(_mlibc_atexit_funcs)();
        vector_pop(_mlibc_atexit_funcs);
    }

    _mlibc_sysexit(exit_code);
}


