#include <internals/mman.h>
#include <internals/syscall-table.h>
#include <stdatomic.h>
#include <internals/file.h>
#include <string.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvoid-pointer-to-int-cast"
#if _MLIBC_WORDSIZE == 8
extern void* _mlibc_syscall_64(uint64_t number, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);
#define _mlibc_syscall _mlibc_syscall_64
typedef uint64_t ptr_t;
#elif _MLIBC_WORDSIZE == 4
#error "32-bit malloc not implemented"
#endif

volatile size_t _mlibc_page_size = 0;
volatile uint8_t _mlibc_page_size_lock = 0;


int __open(const char* path, int flags, mode_t mode) {
    return (int) _mlibc_syscall(SYS_open, (ptr_t)path, flags, mode, 0, 0, 0);
}

int __close(int fd) {
    return (int) _mlibc_syscall(SYS_close, fd, 0, 0, 0, 0, 0);
}

ssize_t __read(int fd, void* buf, size_t count) {
    return (ssize_t) _mlibc_syscall(SYS_read, fd, (ptr_t)buf, count, 0, 0, 0);
}

ssize_t __write(int fd, const void* buf, size_t count) {
    return (ssize_t) _mlibc_syscall(SYS_write, fd, (ptr_t)buf, count, 0, 0, 0);
}

void* __map(void* addr, size_t length, PROT prot, MAP flags, int fd, off_t offset) {
    return _mlibc_syscall(SYS_mmap, (ptr_t)addr, length, prot, flags, fd, offset);
}

int __munmap(void* addr, size_t length) {
    return (int) _mlibc_syscall(SYS_munmap, (ptr_t) addr, length, 0, 0, 0, 0);
}

int __mprotect(void* addr, size_t length, PROT prot) {
    return (int) _mlibc_syscall(SYS_mprotect, (ptr_t)addr, length, prot, 0, 0, 0);
}

size_t __getpagesize() {
    while (atomic_exchange(&_mlibc_page_size_lock, 1) != 0);
    // If the page size has not been set yet, get it from the kernel
    // We can't use sysconf because it is not implemented, yet
    // We will have to read it from /proc/self/smaps
    if (_mlibc_page_size == 0) {
        int fd = __open("/proc/self/smaps", O_RDONLY, 0);
        if (fd < 0) {
            atomic_store(&_mlibc_page_size_lock, 0);
            return 0;
        }
        char buf[4096];
        ssize_t read = __read(fd, buf, 4096);
        __close(fd);
        if (read < 0) {
            atomic_store(&_mlibc_page_size_lock, 0);
            return 0;
        }
        const char* target = "KernelPageSize:\0";
        const uint8_t target_len = strlen(target);
        size_t i = 0;
        while (i < read){
            if (strncmp(buf + i, target, target_len) == 0) {
                i += target_len;
                while (buf[i] == ' ' || buf[i] == '\t') {
                    i++;
                }
                _mlibc_page_size = 0;
                while (buf[i] >= '0' && buf[i] <= '9') {
                    _mlibc_page_size = _mlibc_page_size * 10 + (buf[i] - '0');
                    i++;
                }
                // The page size is in kB, so we need to multiply it by 1024
                _mlibc_page_size *= 1024;
                break;
            }
            i++;
        }
    }
    atomic_store(&_mlibc_page_size_lock, 0);
    return _mlibc_page_size;
}

#pragma clang diagnostic pop