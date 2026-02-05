#include <string.h>
#include <mlibc/macros.h>
#include <stdint.h>
using byte = uint8_t;

EXPORT
void* memcpy(void* __MLIBC_RESTRICT dest, const void* __MLIBC_RESTRICT src, size_t n) {
	const auto d = static_cast<byte*>(dest);
	const auto s = static_cast<const byte*>(src);
	for (size_t i = 0; i < n; ++i) {
		d[i] = s[i];
	}
	return dest;
}

EXPORT
void* memmove(void* __MLIBC_RESTRICT dest, const void* __MLIBC_RESTRICT src, size_t n) {
	const auto d = static_cast<byte*>(dest);
	const auto s = static_cast<const byte*>(src);
	if (d < s) {
		for (size_t i = 0; i < n; ++i) {
			d[i] = s[i];
		}
		return dest;
	}
	for (size_t i = n; i > 0; --i) {
		d[i - 1] = s[i - 1];
	}
	return dest;
}

EXPORT
void* memset(void* s, int c, size_t n) {
	// The spec says that c is converted to unsigned char
	const auto dest = static_cast<unsigned char*>(s);
	const auto value = static_cast<unsigned char>(c);
	for (size_t i = 0; i < n; ++i) {
		dest[i] = value;
	}
	return s;
}
