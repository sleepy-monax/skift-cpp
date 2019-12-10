MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBVIDEO  equ  0x4
FLAGS    equ  MBALIGN | MEMINFO | MBVIDEO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    ; MULTIBOOT_AOUT_KLUDGE
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0

    ; MULTIBOOT_VIDEO_MODE
    dd 1
    dd 1280
    dd 1024
    dd 32


section .bss
align 4096
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    cli
    cld

    mov esp, stack_top
    push 0
    push 0
    mov ebp, esp
    extern arch_main

    push ebx
    push eax

    call arch_main

    cli
.hang:	hlt
    jmp .hang
.end:
