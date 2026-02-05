#include <memory>
#include <stdint.h>
#include <assert.h>

using namespace mlibc;

void* std::align(size_t alignment,
					size_t size,
					void*& ptr,
					size_t& space) {
	assert((alignment & (alignment - 1)) == 0 && "Alignment must be a power of two");

	uintptr_t current_ptr = reinterpret_cast<uintptr_t>(ptr);
	uintptr_t aligned_ptr = (current_ptr + alignment - 1) & ~(alignment - 1);
	size_t padding = aligned_ptr - current_ptr;

	if (space < padding || space - padding < size) {
		return nullptr;
	}

	ptr = reinterpret_cast<void*>(aligned_ptr);
	space -= padding;
	return ptr;
}