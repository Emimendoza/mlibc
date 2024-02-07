#include <stdlib.h>

[[noreturn]] extern void _mlibc_sysexit(int exit_code);

[[noreturn]] void exit(int exit_code) {
    _mlibc_sysexit(exit_code);
}