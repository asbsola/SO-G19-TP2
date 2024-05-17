section .text

global sys_write
global sys_set_font_size

sys_write:
    push rbp
    mov rbp, rsp

    mov rax, 1
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_set_font_size:
    push rbp
    mov rbp, rsp

    mov rax, 2
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
