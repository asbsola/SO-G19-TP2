
#include <screen_service.h>
#include <managers/kernel_managers.h>
#include <drivers/videoDriver.h>

extern void yield();
uint64_t screen_service(char** argv, int argc){
    set_font_size(1);
    clear_video_text_buffer();

    write_to_video_text_buffer("GRUPO 19\n", 9, 0x006fb5fb);
    while(1) {
        char buff[BUFFER_SIZE];
        int len = read_pipe(the_pipes_manager, get_stdin(the_process_manager, get_current_pid(the_scheduler)), buff, BUFFER_SIZE);
        write_to_video_text_buffer(buff, len, 0x00ffffff);
        yield();
    }
}