#pragma once
// This file will determine the word size in bytes of the system

#if defined __x86_64__ && !defined __ILP32__
    #define _MLIBC_WORDSIZE	8
#else
    #define _MLIBC_WORDSIZE	4
#endif
