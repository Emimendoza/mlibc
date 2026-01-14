#pragma once
#include <_mlibc_internal/cpp_start.h>
long __sys0(long n);
long __sys1(long n, ...);
long __sys2(long n, ...);
long __sys3(long n, ...);
long __sys4(long n, ...);
long __sys5(long n, ...);
long __sys6(long n, ...);
#include <_mlibc_internal/cpp_end.h>
template <typename... Args>
long sys(const long number, Args... args) {
	if constexpr (sizeof...(Args) == 0) {
		return __sys0(number);
	} else if constexpr (sizeof...(Args) == 1) {
		return __sys1(number, args...);
	} else if constexpr (sizeof...(Args) == 2) {
		return __sys2(number, args...);
	} else if constexpr (sizeof...(Args) == 3) {
		return __sys3(number, args...);
	} else if constexpr (sizeof...(Args) == 4) {
		return __sys4(number, args...);
	} else if constexpr (sizeof...(Args) == 5) {
		return __sys5(number, args...);
	} else if constexpr (sizeof...(Args) == 6) {
		return __sys6(number, args...);
	} else {
		static_assert(false);
		return -1;
	}
}
