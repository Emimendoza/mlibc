#include <internals/debug.h>
#include <internals/syscall-table.h>
#include <internals/internal-io.h>
#include <internals/file.h>
#include <string.h>
#include <stdlib.h>

[[noreturn]] void __failure(const char* msg){
    __write(_mlibc_stderr, msg, strlen(msg));
    exit(1);
}

[[noreturn]] void __stack_chk_fail(){
    __failure("Stack smashing detected");
}