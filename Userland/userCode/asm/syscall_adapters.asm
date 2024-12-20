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
global sys_get_ticks
global sys_print_registers
global sys_clear_screen
global sys_malloc
global sys_free
global sys_get_usable_memory_size
global sys_get_free_memory_size
global sys_get_total_memory_size
global sys_ps
global sys_create_process
global sys_exit_process_by_pid
global sys_block_process_by_pid
global sys_kill_process_by_pid
global sys_unblock_process_by_pid
global sys_get_pid
global sys_get_ppid
global sys_wait
global sys_wait_pid
global sys_nice
global sys_yield
global sys_sem_open_named
global sys_sem_close_named
global sys_sem_open
global sys_sem_close
global sys_sem_up
global sys_sem_down
global sys_get_stdin
global sys_get_stdout
global sys_pipe_open_named
global sys_pipe_open
global sys_pipe_close
global sys_pipe_send_eof
global sys_get_process_status

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

sys_get_ticks:
    syscall 13

sys_print_registers:
    syscall 14

sys_clear_screen:
    syscall 15

sys_malloc:
    syscall 16

sys_free:
    syscall 17

sys_get_usable_memory_size:
    syscall 18

sys_get_free_memory_size:
    syscall 19

sys_get_total_memory_size:
    syscall 20

sys_ps:
    syscall 21

sys_create_process:
    syscall 22

sys_exit_process_by_pid:
    syscall 23

sys_block_process_by_pid:
    syscall 24

sys_kill_process_by_pid:
    syscall 25

sys_unblock_process_by_pid:
    syscall 26

sys_get_pid:
    syscall 27
    
sys_get_ppid:
    syscall 28

sys_wait:
    syscall 29

sys_wait_pid:
    syscall 30

sys_nice:
    syscall 31

sys_yield:
    syscall 32

sys_sem_open_named:
    syscall 33

sys_sem_close_named:
    syscall 34

sys_sem_open:
    syscall 35

sys_sem_close:
    syscall 36

sys_sem_up:
    syscall 37

sys_sem_down:
    syscall 38

sys_get_stdin:
    syscall 39

sys_get_stdout:
    syscall 40

sys_pipe_open_named:
    syscall 41

sys_pipe_open:
    syscall 42

sys_pipe_close:
    syscall 43

sys_pipe_send_eof:
    syscall 44

sys_get_process_status:
    syscall 45