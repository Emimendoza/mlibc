global _mlibc_sysexit_64

_mlibc_sysexit_64:
    ; Args:
    ; rdi: exit code
    mov rax, 60
    syscall

