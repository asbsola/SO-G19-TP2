#include <std.h>
#include <syscall_adapters.h>

uint32_t strlen(const char* s) {
    uint32_t i;

    for (i = 0; s[i] != '\0'; i++) ;

    return i;
}

void putchar(char c) {
    sys_write(1, &c, 1);
}

void puts(const char* s) {
    sys_write(1, s, strlen(s));
}

char itoa_buff[ITOA_BUFF_MAX_SIZE];
char* itoa(uint64_t num, char* dest, uint32_t dest_max_len) {
    itoa_buff[0] = 0;

    uint32_t i;
    for (i = 0; num != 0; num /= 10)
        itoa_buff[i++] = num % 10;

    if (i == 0) i = 1;

    uint32_t j;
    for (j = 0; j < dest_max_len && j < i; j++)
        dest[j] = itoa_buff[i - j - 1] + '0';

    dest[j] = '\0';

    return dest;
}

char printf_buff[PRINTF_PRINT_BUFF_MAX_SIZE];
void printf(const char* fmt, uint64_t arg) {
    uint32_t k = 0;
    for (uint32_t i = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i + 1] == 's') {
            char* arg_s = (char*)arg;
            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && arg_s[j] != '\0'; j++)
                printf_buff[k++] = arg_s[j];
            i++;
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'c') {
            printf_buff[k++] = (char)arg;
            i++;
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'd') {
            char temp[ITOA_BUFF_MAX_SIZE];
            itoa(arg, temp, ITOA_BUFF_MAX_SIZE);

            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && temp[j] != '\0'; j++)
                printf_buff[k++] = temp[j];

            i++;
        }
        else {
            printf_buff[k++] = fmt[i];
        }
    }
    printf_buff[k] = '\0';
    puts(printf_buff);
}
