global jump_to_dir
global _hlt
section .text

jump_to_dir:
    jmp rdi
    ret

_hlt:
    hlt
    ret
