#pragma once
#include "macros.h"
#include <stdint.h>
#include <stddef.h>

struct auxv_t {
	size_t a_type;
	union {
		size_t a_val;
		void*  a_ptr;
	} a_un;
};

const auxv_t* getauxvect(size_t type);

extern "C" {
EXPORT [[noreturn]]
void __mlibc_init(void*);
}