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
global sys_print_registers
global sys_clear_screen

%macro syscall 1
    push rbp
    mov rbp, rsp

    mov rax, %1 
    mov r10, rcx
    int 0x80

    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_read:
    syscall 0

sys_write:
    syscall 1

sys_put_text:
    syscall 2

sys_set_font_size:
    syscall 3

sys_draw_square:
    syscall 4

sys_get_screen_width:
    syscall 5

sys_get_screen_height:
    syscall 6

sys_get_time:
    syscall 7

sys_get_key_pressed:
    syscall 8

sys_get_character_pressed:
    syscall 9

sys_clear_text_buffer:
    syscall 10

sys_get_cpu_vendor:
    syscall 11

sys_beep:
    syscall 12

sys_delay:
    syscall 13

sys_print_registers:
    syscall 14

sys_clear_screen:
    syscall 15
