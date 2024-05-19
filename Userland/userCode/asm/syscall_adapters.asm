section .text

global sys_write
global sys_set_font_size
global sys_put_text
global sys_draw_square
global sys_get_screen_width
global sys_get_screen_height
global sys_get_time

sys_write:
    push rbp
    mov rbp, rsp

    mov rax, 1
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_put_text:
    push rbp
    mov rbp, rsp

    mov rax, 2
    mov r10, rcx
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_set_font_size:
    push rbp
    mov rbp, rsp

    mov rax, 3
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_draw_square:
    push rbp
    mov rbp, rsp

    mov rax, 4
    mov r10, rcx
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_get_screen_width:
    push rbp
    mov rbp, rsp

    mov rax, 5
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_get_screen_height:
    push rbp
    mov rbp, rsp

    mov rax, 6
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_get_time:
    push rbp
    mov rbp, rsp

    mov rax, 7
    int 0x80

    mov rsp, rbp
    pop rbp
    ret