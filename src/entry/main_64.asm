global _mlibc_main
extern main, _mlibc_entry_64

section .text
_mlibc_main:
mov rcx, [rel main_ptr]
call rcx
ret

section .data
main_ptr: dq main

