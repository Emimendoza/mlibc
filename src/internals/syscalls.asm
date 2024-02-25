global _mlibc_syscall_64, _mlibc_syscall_64_alt

section .text
; We have an alt in case we need to define sys calls with different return widths
_mlibc_syscall_64_alt:
    ; Args:
    ; rdi: syscall number
    ; rsi: arg1
    ; rdx: arg2
    ; rcx: arg3
    ; r8: arg4
    ; r9: arg5
    ; the arg6 is passed on the stack
_mlibc_syscall_64:
    ; Args:
    ; rdi: syscall number
    ; rsi: arg1
    ; rdx: arg2
    ; rcx: arg3
    ; r8: arg4
    ; r9: arg5
    ; the arg6 is passed on the stack

mov rax, rdi
mov rdi, rsi
mov rsi, rdx
mov rdx, rcx
mov r10, r8
mov r8, r9
mov r9, [rsp + 8]
syscall
ret


