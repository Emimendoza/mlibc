#pragma once
#define EXPORT __attribute__((visibility("default")))
#define WEAK __attribute__((weak))
#ifdef __has_builtin
	#if __has_builtin(__builtin_unreachable)
		#define UNREACHABLE() __builtin_unreachable()
	#else
		#define UNREACHABLE() do {} while (1)
	#endif
#else
#warning "Cannot detect builtin support, assuming there are no builtins"
	#define UNREACHABLE() do {} while (1)
#endif
#define CEXPORT extern "C" EXPORT