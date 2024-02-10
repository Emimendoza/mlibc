#include <string.h>
#include <internals/bits/wordsize.h>

// Will chose one of:
//  _mlibc_memcpy_512, _mlibc_memcpy_256, _mlibc_memcpy_128, _mlibc_memcpy_64, _mlibc_memcpy_32
// Depending on AVX512, AVX2, SSE2, or x86-64

// Checking availability of AVX512
#if defined(__AVX512F__) && defined(__AVX512DQ__) && defined(__AVX512BW__) && defined(__AVX512VL__)
extern void* _mlibc_memcpy_512(void* dest, const void* src, size_t n);
#define __memcpy _mlibc_memcpy_512
// Checking availability of AVX2
#elif defined(__AVX2__)
extern void* _mlibc_memcpy_256(void* dest, const void* src, size_t n);
#define __memcpy _mlibc_memcpy_256
// Checking availability of SSE2
#elif defined(__SSE2__)
extern void* _mlibc_memcpy_128(void* dest, const void* src, size_t n);
#define __memcpy _mlibc_memcpy_128
// Fallback to x86-64
#elif _MLIBC_WORDSIZE == 8
extern void* _mlibc_memcpy_64(void* dest, const void* src, size_t n);
#define __memcpy _mlibc_memcpy_64
// Fallback to x86-32
#else
extern void* _mlibc_memcpy_32(void* dest, const void* src, size_t n);
#define __memcpy _mlibc_memcpy_32
#endif

void* memcpy(void* dest, const void* src, size_t n) {
    return __memcpy(dest, src, n);
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int strncmp(const char* str1, const char* str2, size_t n) {
    while (n && *str1 && *str1 == *str2) {
        str1++;
        str2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* strchr(const char* str, int c) {
    while (*str != (char)c) {
        if (!*str) {
            return NULL;
        }
        str++;
    }
    return (char*)str;
}