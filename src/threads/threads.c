#include <threads.h>
#include <internals/threads.h>
#include <stddef.h>

__thread int __mlibc_errno;
__thread size_t __mlibc_thread_id;