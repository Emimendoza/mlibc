global _mlibc_memcpy_512, _mlibc_memcpy_256, _mlibc_memcpy_128, _mlibc_memcpy_64, _mlibc_memcpy_32

section .text

; Will use 512-bit registers to copy memory (AVX-512)
_mlibc_memcpy_512:
    ; rdi = dest, rsi = src, rdx = len
xor r8, r8 ; offset
xor r9, r9 ; left
xor rax, rax
.loop:
mov r9, rdx
sub r9, r8
cmp r9, 0
je .end
cmp r9, 64
jae short .copy_512
cmp r9, 32
jae short .copy_256
cmp r9, 16
jae short .copy_128
cmp r9, 8
jae short .copy_64
jmp short .copy_8
.copy_512:
vmovdqu64 zmm0, [rsi + r8]
vmovdqu64 [rdi + r8], zmm0
add r8, 64
jmp short .loop
.copy_256:
vmovdqu ymm0, [rsi + r8]
vmovdqu [rdi + r8], ymm0
add r8, 32
jmp short .loop
.copy_128:
vmovdqu xmm0, [rsi + r8]
vmovdqu [rdi + r8], xmm0
add r8, 16
jmp short .loop
.copy_64:
mov rax, [rsi + r8]
mov [rdi + r8], rax
add r8, 8
jmp short .loop
.copy_8:
mov al, [rsi + r8]
mov [rdi + r8], al
inc r8
jmp short .loop
.end:
mov rax, rdi
ret

; Will use 256-bit registers to copy memory (AVX2)
; Using vmovdqu
_mlibc_memcpy_256:
    ; rdi = dest, rsi = src, rdx = len
xor r8, r8 ; offset
xor r9, r9 ; left
xor rax, rax
.loop:
mov r9, rdx
sub r9, r8
cmp r9, 0
je .end
cmp r9, 32
jae short .copy_256
cmp r9, 16
jae short .copy_128
cmp r9, 8
jae short .copy_64
jmp short .copy_8
.copy_256:
vmovdqu ymm0, [rsi + r8]
vmovdqu [rdi + r8], ymm0
add r8, 32
jmp short .loop
.copy_128:
vmovdqu xmm0, [rsi + r8]
vmovdqu [rdi + r8], xmm0
add r8, 16
jmp short .loop
.copy_64:
mov rax, [rsi + r8]
mov [rdi + r8], rax
add r8, 8
jmp short .loop
.copy_8:
mov al, [rsi + r8]
mov [rdi + r8], al
inc r8
jmp short .loop
.end:
mov rax, rdi
ret

; Will use 128-bit registers to copy memory (SSE2)
; Using movdqu
_mlibc_memcpy_128:
    ; rdi = dest, rsi = src, rdx = len
xor r8, r8 ; offset
xor r9, r9 ; left
xor rax, rax
.loop:
mov r9, rdx
sub r9, r8
cmp r9, 0
je .end
cmp r9, 16
jae short .copy_128
cmp r9, 8
jae short .copy_64
jmp short .copy_8
.copy_128:
movdqu xmm0, [rsi + r8]
movdqu [rdi + r8], xmm0
add r8, 16
jmp short .loop
.copy_64:
mov rax, [rsi + r8]
mov [rdi + r8], rax
add r8, 8
jmp short .loop
.copy_8:
mov al, [rsi + r8]
mov [rdi + r8], al
inc r8
jmp short .loop
.end:
mov rax, rdi
ret


; Will use 64-bit registers to copy memory
_mlibc_memcpy_64:
    ; rdi = dest, rsi = src, rdx = len
xor r8, r8 ; offset
xor r9, r9 ; left
xor rax, rax
.loop:
mov r9, rdx
sub r9, r8
cmp r9, 0
je .end
cmp r9, 8
jae short .copy_64
jmp short .copy_8
.copy_64:
mov rax, [rsi + r8]
mov [rdi + r8], rax
add r8, 8
jmp short .loop
.copy_8:
mov al, [rsi + r8]
mov [rdi + r8], al
inc r8
jmp short .loop
.end:
mov rax, rdi
ret

; Will use 32-bit registers to copy memory ( for x86 )
_mlibc_memcpy_32:
xor ecx, ecx ; offset
xor eax, eax ; left
.loop:
mov edx, [esp + 4]
sub edx, ecx
cmp edx, 0
je .end
cmp edx, 4
jae short .copy_32
jmp short .copy_8
.copy_32:
mov eax, [esi + ecx]
mov [edi + ecx], eax
add ecx, 4
jmp short .loop
.copy_8:
mov al, [esi + ecx]
mov [edi + ecx], al
inc ecx
jmp short .loop
.end:
mov eax, edi
ret

