
; entry.asm
;
; Antoine LANDRIEUX
; BORIUM <https://github.com/AntoineLandrieux/BORIUM>
;
; MIT License

[bits 32]
[section .multiboot_header]
align 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

[section .text]
global _start
extern start

_start:
    mov esp, 0x90000
    call start
