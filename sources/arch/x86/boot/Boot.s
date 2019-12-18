; Copyright © 2019-2020 N. Van Bossuyt.                                        ;
; This code is licensed under the 3-Clause BSD License.                        ;
; See: LICENSE.md                                                              ;

MULTIBOOT2_HEADER_MAGIC equ 0xe85250d6
MULTIBOOT2_ARCHITECTURE_I386 equ 0
MULTIBOOT2_HEADER_LENGHT equ __multiboot_header_end - __multiboot_header_start
MULTIBOOT2_CHECKSUM equ -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_ARCHITECTURE_I386 + MULTIBOOT2_HEADER_LENGHT)

MULTIBOOT2_HEADER_TAG_END equ 0
MULTIBOOT2_HEADER_TAG_INFORMATION_REQUEST equ 1
MULTIBOOT2_HEADER_TAG_ADDRESS equ 2
MULTIBOOT2_HEADER_TAG_ENTRY_ADDRESS equ 3
MULTIBOOT2_HEADER_TAG_CONSOLE_FLAGS equ 4
MULTIBOOT2_HEADER_TAG_FRAMEBUFFER equ 5
MULTIBOOT2_HEADER_TAG_MODULE_ALIGN equ 6
MULTIBOOT2_HEADER_TAG_EFI_BS equ 7
MULTIBOOT2_HEADER_TAG_ENTRY_ADDRESS_EFI32 equ 8
MULTIBOOT2_HEADER_TAG_ENTRY_ADDRESS_EFI64 equ 9
MULTIBOOT2_HEADER_TAG_RELOCATABLE equ 10

section .multiboot
__multiboot_header_start:

align 4
    dd MULTIBOOT2_HEADER_MAGIC
    dd MULTIBOOT2_ARCHITECTURE_I386
    dd MULTIBOOT2_HEADER_LENGHT
    dd MULTIBOOT2_CHECKSUM

; END HEADER TAG
    dw MULTIBOOT2_HEADER_TAG_END
    dw 0
    dw 8

__multiboot_header_end:



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
