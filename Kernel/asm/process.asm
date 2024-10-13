section .text

global start_process_wrapper 
global yield
extern start_process

start_process_wrapper:
    mov rdi, rsp
    call start_process

yield:
    push rbp
    mov rbp, rsp

    int 0x20

    mov rsp, rbp
    pop rbp
    ret
