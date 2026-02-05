#include <mlibc/elf.h>
#include <mlibc/entry.h>
#include <linux/auxvec.h>
#include <assert.h>

using namespace mlibc;


const NElf_Phdr* mlibc::find_phdr(NElf_Word p_type, nelf_phdr_t* phdr) {
	if (!phdr) {
		static nelf_phdr_t phdr_ = [] {
			const auto aux_phdr = getauxvect(AT_PHDR);
			const auto aux_phnum = getauxvect(AT_PHNUM);
			const auto aux_phent = getauxvect(AT_PHENT);
			assert (aux_phdr && aux_phnum && aux_phent &&
				"Auxiliary vector missing program header information");
			assert(sizeof(NElf_Phdr) == aux_phent->a_un.a_val &&
				"Program header entry size mismatch");
			return nelf_phdr_t{
				.phdrs = static_cast<NElf_Phdr*>(aux_phdr->a_un.a_ptr),
				.phnum = static_cast<NElf_Half>(aux_phnum->a_un.a_val)
			};
		}();
		phdr = &phdr_;
	}

	for (NElf_Half i = 0; i < phdr->phnum; ++i) {
		if (phdr->phdrs[i].p_type == p_type) {
			return &phdr->phdrs[i];
		}
	}

	return nullptr;
}
