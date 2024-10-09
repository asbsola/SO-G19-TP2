section .text

global start_process_wrapper 
global go_to_scheduler
extern start_process

start_process_wrapper:
    mov rdi, rsp
    call start_process

go_to_scheduler:
    push rbp
    mov rbp, rsp

    int 0x20

    mov rsp, rbp
    pop rbp
    ret
