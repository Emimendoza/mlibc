#include <stdlib.h>
#include <mlibc/macros.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>


template<typename T>
constexpr INLINE
T str_to_int(const char* __MLIBC_RESTRICT str, char** __MLIBC_RESTRICT str_end, int base) {
	T ret = 0;
	const char* ptr = str;
	bool signed_number = false;
	bool parsed = false;

	while (*ptr != '\0') {
		if (isspace(*ptr)) {
			++ptr;
			continue;
		}
		if (*ptr == '-') {
			signed_number = true;
			++ptr;
			break;
		}
		if (*ptr == '+') {
			++ptr;
			break;
		}
	}

	if (base == 0) {
		if (*ptr == '0' && (ptr[1] == 'x' || ptr[1] == 'X')) {
			base = 16;
			ptr += 2;
		} else if (*ptr == '0') {
			base = 8;
			++ptr;
		} else {
			base = 10;
		}
	}

	auto prev_ret = ret;
	while (*ptr != '\0') {
		int digit;
		if (*ptr >= '0' && *ptr <= '9') {
			digit = *ptr - '0';
		} else if (*ptr >= 'a' && *ptr <= 'z') {
			digit = *ptr - 'a' + 10;
		} else if (*ptr >= 'A' && *ptr <= 'Z') {
			digit = *ptr - 'A' + 10;
		} else {
			break;
		}
		if (digit >= base) {
			break;
		}
		parsed = true;
		prev_ret = ret;
		ret = ret * base + digit;
		++ptr;
		if (ret < prev_ret) {
			// Overflow
			errno = ERANGE;
			if (str_end != nullptr) {
				*str_end = const_cast<char*>(ptr);
			}
			return __MLIBC_MAX_U(T);
		}
	}
	if (!parsed) {
		if (str_end != nullptr) {
			*str_end = const_cast<char*>(str);
		}
		return 0;
	}

	if (signed_number) {
		ret = -ret;
	}

	if (str_end != nullptr) {
		*str_end = const_cast<char*>(ptr);
	}
	return ret;
}

EXPORT
unsigned long strtoul(const char* __MLIBC_RESTRICT str, char** __MLIBC_RESTRICT str_end, int base) {
	return str_to_int<unsigned long>(str, str_end, base);
}
