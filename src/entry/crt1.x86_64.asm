global _start
extern __mlibc_init
section .text
_start:
mov rdi, rsp
jmp __mlibc_init
