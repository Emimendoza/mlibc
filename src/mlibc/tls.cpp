#include <mlibc/macros.h>
#include <mlibc/variables.h>
#include <mlibc/syscall.h>
#include <cstddef>
#include <array>
#include <sys/mman.h>
#include <asm/prctl.h>
#include <linux/auxvec.h>
#include <mlibc/entry.h>
#include <mlibc/elf.h>
#include <string.h>
#include <assert.h>

using namespace mlibc;
using std::byte;

// I think the dynamic loader provides this symbol,
// but I am not sure
/*
CEXPORT WEAK
void* __tls_get_addr(void* desc) {
	return nullptr;
}
*/

thread_local const pid_t mlibc::pid = sys(SYS_getpid);
thread_local const pid_t mlibc::tid = sys(SYS_gettid);




struct dtv_header {
	size_t total_size;
	std::array<byte*, 32> slots;
};

struct dtv {
	size_t counter;
	size_t total_size;
	byte* data_start;
	byte* data_current;
	byte* data_end;
	constexpr byte*& operator[] (const size_t index) const noexcept {
		assert(index <= counter);
		return ((byte**)this)[-(ssize_t)index];
	}
	constexpr byte*& operator[] (const size_t index) noexcept {
		assert(index <= counter);
		return ((byte**)this)[-(ssize_t)index];
	}
	constexpr byte*& allocate_slot(const size_t size, const size_t alignment = 1) noexcept {
		assert(counter <= total_size);
		assert(alignment > 0 && (alignment & (alignment - 1)) == 0); // power of two
		data_current = reinterpret_cast<byte*>(
			(reinterpret_cast<size_t>(data_current) + alignment - 1) & ~(alignment - 1)
		);
		byte* allocation = data_current;
		data_current += size;
		assert(data_current <= data_end);
		return (*this)[counter++] = allocation;
	}
};

struct tcb {
	tcb* _self;
	dtv* _dtv;
	uintptr_t _padding[2];
	uintptr_t canary;
};



void mlibc::init_tls() {
	// TODO: Do this properly
	if (dynamically_linked) {
		// Let the dynamic linker handle TLS initialization
		return;
	}

	auto* tls_desc = find_phdr(PT_TLS);
	if (!tls_desc) {
		// No TLS segment
		// WHICH SHOULDN'T HAPPEN BECAUSE WE (THE LIBC) USE TLS
		assert(false);
		return;
	}

	void* initial_data = reinterpret_cast<void*>(tls_desc->p_vaddr);
	size_t initial_data_size = tls_desc->p_filesz;
	size_t total_size = tls_desc->p_memsz;
	size_t alignment = tls_desc->p_align;
	auto total_size_aligned = (total_size + alignment - 1) & ~(alignment - 1);

	size_t memory_needed = total_size_aligned + sizeof(tcb);
	memory_needed = (memory_needed + page_size -1 ) & ~(page_size -1);
	const size_t tcb_offset = memory_needed;
	// for dynamic modules (though shouldn't be needed since we're statically linked).
	const size_t dynamic_tls_size = page_size;
	memory_needed += dynamic_tls_size;

	// DTV will be allocated on its own page
	size_t dtv_memory = sizeof(dtv) + (2 * sizeof(void*)); // one slot for main program, one for libc
	dtv_memory = (dtv_memory + page_size -1 ) & ~(page_size -1);


	auto* alloc = reinterpret_cast<byte*>(sys(SYS_mmap, nullptr, memory_needed, PROT_READ | PROT_WRITE,
							  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
	if (alloc == MAP_FAILED) {
		__mlibc_assert_fail("TLS initialization failed", __FILE__, __LINE__);
	}
	// One page for dtv pointers
	auto* alloc_2 = reinterpret_cast<byte*>(sys(SYS_mmap, nullptr, dtv_memory, PROT_READ | PROT_WRITE,
							  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

	if (alloc_2 == MAP_FAILED) {
		__mlibc_assert_fail("TLS initialization failed", __FILE__, __LINE__);
	}


	auto* tls = reinterpret_cast<tcb*>(alloc+tcb_offset);
	tls->_dtv = reinterpret_cast<dtv*>(alloc_2+dtv_memory - sizeof(dtv));
	tls->_self = tls;
	tls->_dtv->counter = 1;
	tls->_dtv->total_size = (dtv_memory-sizeof(dtv))/sizeof(void*);
	tls->_dtv->data_start = alloc+tcb_offset+sizeof(tcb);
	tls->_dtv->data_current = tls->_dtv->data_start;
	tls->_dtv->data_end = alloc+tcb_offset+dynamic_tls_size;

	auto& main_program_tls = (*tls->_dtv)[1];
	// right before the start of tcb
	main_program_tls = alloc+tcb_offset-total_size_aligned;

	// copy initial data
	memcpy(main_program_tls, initial_data, initial_data_size);
	// zero the rest
	memset(main_program_tls + initial_data_size, 0, total_size - initial_data_size);

	// set fs to tls
	sys(SYS_arch_prctl, ARCH_SET_FS, tls);

}
