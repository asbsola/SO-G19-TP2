section .text

global start_shell
global restart_shell

global SHELL_CODE_ADDRESS
global SHELL_DATA_ADDRESS

start_shell:
    mov [shell_start_rsp], rsp

restart_shell:
    push qword 0x0
    push qword [shell_start_rsp]
    push qword 0x202
    push qword 0x8
    push qword [SHELL_CODE_ADDRESS]
    iretq

section .data
    shell_start_rsp dq 0

section .rodata
    SHELL_CODE_ADDRESS dq 0xA00000
    SHELL_DATA_ADDRESS dq 0xB00000
