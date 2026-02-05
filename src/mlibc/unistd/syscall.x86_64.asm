global syscall, __sys0:hidden, __sys1:hidden, __sys2:hidden, __sys3:hidden, __sys4:hidden, __sys5:hidden, __sys6:hidden

section .text.syscall exec
syscall:
__sys6:
mov rax, rdi
mov rdi, rsi
mov rsi, rdx
mov rdx, rcx
mov r10, r8
mov r8, r9
mov r9, [rsp+8]
syscall
ret
section .text.sys5 exec
__sys5:
mov rax, rdi
mov rdi, rsi
mov rsi, rdx
mov rdx, rcx
mov r10, r8
mov r8, r9
syscall
ret
section .text.sys4 exec
__sys4:
mov rax, rdi
mov rdi, rsi
mov rsi, rdx
mov rdx, rcx
mov r10, r8
syscall
ret
section .text.sys3 exec
__sys3:
mov rax, rdi
mov rdi, rsi
mov rsi, rdx
mov rdx, rcx
syscall
ret
section .text.sys2 exec
__sys2:
mov rax, rdi
mov rdi, rsi
mov rsi, rdx
syscall
ret
section .text.sys1 exec
__sys1:
mov rax, rdi
mov rdi, rsi
syscall
ret

section .text.sys0 exec
__sys0:
mov rax, rdi
syscall
ret