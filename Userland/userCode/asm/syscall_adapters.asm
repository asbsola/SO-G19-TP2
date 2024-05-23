section .text

global sys_read
global sys_write
global sys_set_font_size
global sys_put_text
global sys_draw_square
global sys_get_screen_width
global sys_get_screen_height
global sys_get_time
global sys_get_key_pressed
global sys_get_character_pressed
global sys_clear_text_buffer
global sys_get_cpu_vendor
global sys_beep
global sys_delay

sys_read:
    push rbp
    mov rbp, rsp

    mov rax, 0
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

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

sys_get_key_pressed:
    push rbp
    mov rbp, rsp

    mov rax, 8
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_get_character_pressed:
    push rbp
    mov rbp, rsp

    mov rax, 9
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_clear_text_buffer:
    push rbp
    mov rbp, rsp

    mov rax, 10 
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_get_cpu_vendor:
    push rbp
    mov rbp, rsp

    mov rax, 11
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_beep:
    push rbp
    mov rbp, rsp

    mov rax, 12
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_delay:
    push rbp
    mov rbp, rsp

    mov rax, 13
    int 0x80

    mov rsp, rbp
    pop rbp
    ret

sys_clear_screen:
    push rbp
    mov rbp, rsp

    mov rax, 14
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
