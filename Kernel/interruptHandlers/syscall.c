// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <managers/pipesManager.h>
#include <stdint.h>
#include <interruptHandlers/syscall.h>
#include <interruptHandlers/interrupts.h>
#include <drivers/pitDriver.h>
#include <drivers/videoDriver.h>
#include <drivers/rtcDriver.h>
#include <drivers/keyboardDriver.h>
#include <drivers/soundDriver.h>
#include <interruptHandlers/interrupts.h>
#include <lib.h>

#include <managers/kernel_managers.h>

extern void yield();

uint64_t sys_read(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    if(rdi == KEYBOARD_INPUT_FD) set_input_mode(CANNONICAL);
    int len = read_pipe(the_pipes_manager, rdi, (char *)rsi, rdx);
    if(rdi == KEYBOARD_INPUT_FD) set_input_mode(NON_CANNONICAL);
    return len;
}

uint64_t sys_write(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return write_pipe(the_pipes_manager, rdi, (const char *)rsi, rdx);
}

uint64_t sys_set_font_size(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    set_font_size(rdi);
    return 0;
}

uint64_t sys_put_text(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    draw_string((char *)rdi, rsi, rdx, r10, r8);
    return 0;
}

uint64_t sys_draw_square(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    draw_square(rdi, rsi, rdx, r10);
    return 0;
}

uint64_t sys_get_screen_width(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_video_buffer_width();
}

uint64_t sys_get_screen_height(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_video_buffer_height();
}

uint64_t sys_get_time(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return (uint64_t)get_time((int)rdi);
}

uint64_t sys_get_key_pressed(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_key_pending();
}

uint64_t sys_get_character_pressed(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_character_pending();
}

uint64_t sys_clear_text_buffer(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    clear_video_text_buffer();
    return 0;
}

uint64_t sys_get_cpu_vendor(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    cpuVendor((char *)rdi);
    return 0;
}

uint64_t sys_beep(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    beep(rdi, rsi);
    return 0;
}

uint64_t sys_get_ticks(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return ticks_elapsed();
}

uint64_t sys_clear_screen(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    clear_screen(rdi);
    return 0;
}

uint64_t sys_print_registers(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    print_stored_registers();
    return 0;
}

uint64_t sys_malloc(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return (uint64_t)mem_alloc(the_memory_manager, rdi);
}

uint64_t sys_free(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    mem_free(the_memory_manager, (void *)rdi);
    return 0;
}

uint64_t sys_get_usable_memory_size(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_usable_memory_size(the_memory_manager);
}

uint64_t sys_get_free_memory_size(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_free_memory_size(the_memory_manager);
}

uint64_t sys_get_total_memory_size(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_total_memory_size(the_memory_manager);
}

uint64_t sys_ps(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9)
{
    return get_ps_data(the_process_manager, the_memory_manager);
}

uint64_t sys_create_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return create_process(the_process_manager, get_current_pid(the_scheduler), (uint64_t (*)(char**, int))rdi, (char**)rsi, rdx, r10);
}

uint64_t sys_exit_process_by_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return exit_process(the_process_manager, rdi, rsi);
}

uint64_t sys_block_process_by_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return block_process(the_process_manager, rdi);
}

uint64_t sys_kill_process_by_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return kill_process(the_process_manager, rdi, rsi);
}

uint64_t sys_unblock_process_by_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return unblock_process(the_process_manager, rdi);
}

uint64_t sys_get_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return get_current_pid(the_scheduler);
}

uint64_t sys_wait(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return wait(the_process_manager, (int64_t*)rdi); 
}

uint64_t sys_wait_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    return wait_process(the_process_manager, rdi, (int64_t*)rsi); 
}

uint64_t sys_nicent(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return nicent(the_process_manager, rdi, rsi);
}

uint64_t sys_yield(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    yield();
    return 0;
}

uint64_t sys_sem_open_named(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return open_sem_named(the_semaphore_manager, rsi, (char *)rdi);
}

uint64_t sys_sem_close_named(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return close_sem_named(the_semaphore_manager, (char *)rdi);
}

uint64_t sys_sem_open(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return open_sem(the_semaphore_manager, rdi);
}

uint64_t sys_sem_close(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return close_sem(the_semaphore_manager, (sem_t)rdi);
}

uint64_t sys_sem_up(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return up_sem(the_semaphore_manager, (sem_t)rdi);
}

uint64_t sys_sem_down(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return down_sem(the_semaphore_manager, (sem_t)rdi);
}

uint64_t sys_get_stdin(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return get_stdin(the_process_manager, get_current_pid(the_scheduler));
}

uint64_t sys_get_stdout(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return get_stdout(the_process_manager, get_current_pid(the_scheduler));
}

uint64_t sys_pipe_open_named(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return open_pipe_named(the_pipes_manager, (char *)rdi, rsi);
}

uint64_t sys_pipe_open(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return open_pipe(the_pipes_manager, rdi);
}

uint64_t sys_pipe_close(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return close_pipe(the_pipes_manager, rdi);
}

uint64_t sys_pipe_send_eof(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
    return send_eof(the_pipes_manager, rdi);
}

uint64_t (*syscalls[])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t) = {
    sys_read, sys_write, sys_put_text,
    sys_set_font_size, sys_draw_square, sys_get_screen_width,
    sys_get_screen_height, sys_get_time, sys_get_key_pressed, sys_get_character_pressed,
    sys_clear_text_buffer, sys_get_cpu_vendor, sys_beep, sys_get_ticks,
    sys_print_registers, sys_clear_screen,
    sys_malloc, sys_free, sys_get_usable_memory_size, sys_get_free_memory_size, sys_get_total_memory_size, 
    sys_ps, sys_create_process, sys_exit_process_by_pid, sys_block_process_by_pid, sys_kill_process_by_pid, sys_unblock_process_by_pid,
    sys_get_pid, sys_wait, sys_wait_pid, sys_nicent, sys_yield, 
    sys_sem_open_named, sys_sem_close_named, sys_sem_open, sys_sem_close, sys_sem_up, sys_sem_down,
    sys_get_stdin, sys_get_stdout,
    sys_pipe_open_named, sys_pipe_open, sys_pipe_close, sys_pipe_send_eof
};

uint64_t syscall_handler(const registers64_t *registers)
{
    if (registers->rax >= sizeof(syscalls) / sizeof(syscalls[0]))
        return 0;

    return syscalls[registers->rax](registers->rdi, registers->rsi, registers->rdx,
                                    registers->r10, registers->r8, registers->r9);
}
