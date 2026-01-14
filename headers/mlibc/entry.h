#pragma once
#include "macros.h"
#include <stdint.h>
#include <stddef.h>

struct auxv_t {
	size_t a_type;
	union {
		size_t a_val;
	} a_un;
};

extern auxv_t* mlibc_auxv[];

EXPORT [[noreturn]]
void __mlibc_init(void*);