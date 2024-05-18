#include <stdint.h>
#include <syscall.h>
#include <drivers/videoDriver.h>

uint64_t sys_read(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return 0;
}

uint64_t sys_write(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    switch (rdi) {
        case 1:
            write_to_video_text_buffer((char*)rsi, rdx, HEX_WHITE);
            return 0;
        case 2:
            write_to_video_text_buffer((char*)rsi, rdx, HEX_RED);
            return 0;
        default:
            return 1;
    }

    return 1;
}

uint64_t sys_set_font_size(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    set_font_size(rdi);
    return 0;
}

uint64_t (*syscalls[])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t) = {sys_read, sys_write, sys_set_font_size};

uint64_t syscall_handler(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    if (rax >= sizeof(syscalls) / sizeof(syscalls[0]))
            return 0;

    return syscalls[rax](rdi, rsi, rdx, r10, r8, r9);
}