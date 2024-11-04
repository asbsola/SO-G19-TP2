#ifndef __syscall_adapters__
#define __syscall_adapters__

#include <stdint.h>
#include <def.h>

uint64_t sys_read(const char *buff, uint64_t len);
uint64_t sys_write(uint32_t hexColor, const char *buff, uint64_t len);
void sys_put_text(const char *str, uint32_t len, uint32_t hexColor, uint32_t posX, uint32_t posY);
void sys_set_font_size(uint32_t font_size);
void sys_draw_square(uint32_t hexColor, uint32_t posX, uint32_t posY, uint32_t size);
char *sys_get_time(int time_zone);
uint32_t sys_get_screen_width();
uint32_t sys_get_screen_height();
uint8_t sys_get_key_pressed();
char sys_get_character_pressed();
void sys_clear_text_buffer();
void sys_get_cpu_vendor(char *buff);
void sys_beep(uint64_t freq, uint64_t milis);
uint64_t sys_get_ticks();
void sys_clear_screen(uint32_t clearColor);
void sys_print_registers();
void *sys_malloc(uint64_t size);
void sys_free(void *ptr);
uint64_t sys_get_usable_memory_size();
uint64_t sys_get_free_memory_size();
uint64_t sys_get_total_memory_size();
void * sys_ps();
pid_t sys_create_process(uint64_t (*process_start)(char**, int), char** argv);
int sys_exit_process_by_pid(pid_t pid, int64_t status);
int sys_block_process_by_pid(pid_t pid);
int sys_kill_process_by_pid(pid_t pid, uint64_t recursive);
int sys_unblock_process_by_pid(pid_t pid);
pid_t sys_get_pid();
int64_t sys_wait(int64_t* ret);
int64_t sys_wait_pid(pid_t pid, int64_t* ret);
uint64_t sys_nicent(pid_t pid, processPriority priority);
void sys_yield();
int64_t sys_sem_open_named(char* name, uint64_t value);
int64_t sys_sem_close_named(char* name);
int64_t sys_sem_up_named(char* name);
int64_t sys_sem_down_named(char* name);

sem_t sys_sem_open(uint64_t value);
int64_t sys_sem_close(sem_t sem);
int64_t sys_sem_up(sem_t sem);
int64_t sys_sem_down(sem_t sem);
#endif
