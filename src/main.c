// This function defines the entry point for programs using this library

#include <stdlib.h>

extern int _mlibc_main(int argc, char** argv);

[[noreturn]] void _mlibc_entry_64(int argc, char** argv) {
    // Any initialization code can be added here
    int exit_code = _mlibc_main(argc, argv);
    // Any cleanup code can be added here
    exit(exit_code);
}