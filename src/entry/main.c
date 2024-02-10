// This function defines the entry point for programs using this library

#include <stdlib.h>
#include <internals/vector.h>

typedef void (*atexit_func)(void);

extern int _mlibc_main(int argc, char** argv);
extern volatile vector(atexit_func) _mlibc_atexit_funcs;

[[noreturn]] void _mlibc_entry_64(int argc, char** argv) {
    // Any initialization code can be added here
    if(vector_init(_mlibc_atexit_funcs) != 0){
        exit(1); // Out of memory
    }
    int exit_code = _mlibc_main(argc, argv);
    // Any cleanup code can be added here
    exit(exit_code);
}