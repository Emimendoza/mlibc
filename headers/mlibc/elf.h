#pragma once
#include <linux/elf.h>
#include <_mlibc_internal/config.h>

namespace mlibc {
// NELF meaning "Native ELF"

#ifdef __MLIBC_64
using NElf_Phdr = Elf64_Phdr;
using NElf_Word = Elf64_Word;
using NElf_Half = Elf64_Half;
using NElf_Ehdr = Elf64_Ehdr;
using NElf_Sym = Elf64_Sym;
#else
using NElf_Phdr = Elf32_Phdr;
using NElf_Word = Elf32_Word;
using Nelf_Half = Elf32_Half;
using NElf_Ehdr = Elf32_Ehdr;
using NElf_Sym = Elf32_Sym;
#endif

struct nelf_phdr_t {
	NElf_Phdr* phdrs;
	NElf_Half phnum;
};

// phdr = nullptr means to get it from the auxiliary vector
const NElf_Phdr* find_phdr(NElf_Word p_type, nelf_phdr_t* phdr = nullptr);

}