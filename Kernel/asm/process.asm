section .text

global start_wrapper 
global go_to_scheduler
extern start

start_wrapper:
    mov rdi, rsp
    call start 

go_to_scheduler:
    push rbp
    mov rbp, rsp

    int 0x20

    mov rsp, rbp
    pop rbp
    ret
