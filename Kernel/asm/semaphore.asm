section .text

global acquire
global release
extern yield

acquire:
    push rbp
    mov rbp, rsp

    mov al, 0
.acquire_retry:
    xchg byte [rdi], al
    test al, al
    jnz .acquire_done

    call yield

    jmp .acquire_retry 
.acquire_done:
    mov rsp, rbp
    pop rbp
    ret

release:
    mov byte [rdi], 1
    ret

