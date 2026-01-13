#pragma once
#include <_mlibc_internal/cpp_start.h>
long __syscall0(long n);
long __syscall1(long n, ...);
long __syscall2(long n, ...);
long __syscall3(long n, ...);
long __syscall4(long n, ...);
long __syscall5(long n, ...);
long __syscall6(long n, ...);
#include <_mlibc_internal/cpp_end.h>
template <typename... Args>
long sys(const long number, Args... args) {
	if constexpr (sizeof...(Args) == 0) {
		return __syscall0(number);
	} else if constexpr (sizeof...(Args) == 1) {
		return __syscall1(number, args...);
	} else if constexpr (sizeof...(Args) == 2) {
		return __syscall2(number, args...);
	} else if constexpr (sizeof...(Args) == 3) {
		return __syscall3(number, args...);
	} else if constexpr (sizeof...(Args) == 4) {
		return __syscall4(number, args...);
	} else if constexpr (sizeof...(Args) == 5) {
		return __syscall5(number, args...);
	} else if constexpr (sizeof...(Args) == 6) {
		return __syscall6(number, args...);
	} else {
		static_assert(false);
		return -1;
	}
}
