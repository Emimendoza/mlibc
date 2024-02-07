global _start
extern _mlibc_entry_64

_start:
mov rcx, [rel entry_ptr]
call rcx

section .data
entry_ptr dq _mlibc_entry_64