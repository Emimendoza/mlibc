#pragma once

// TODO: get C version we're targeting

#define __MLIBC_NORETURN [[noreturn]]
#ifdef __cplusplus
#define __MLIBC_RESTRICT __restrict__
#define __MLIBC_NOEXCEPT noexcept
#else
#define __MLIBC_RESTRICT restrict
#define __MLIBC_NOEXCEPT
#endif

#define __MLIBC_MIN_U(type) ((type)0)
#define __MLIBC_MAX_U(type) (~__MLIBC_MIN_U(type))
#define __MLIBC_MIN_S(type) __MLIBC_MAX_U(type)
#define __MLIBC_MAX_S(type) (__MLIBC_MIN_S(type)-1)
