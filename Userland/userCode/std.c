#include <std.h>
#include <syscall_adapters.h>
#include <stdarg.h>

uint32_t strcmp(const char* s1, const char* s2) {
    uint32_t i;
    for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
        if (s1[i] != s2[i])
            return s1[i] - s2[i];

    if (s1[i] == '\0' && s2[i] == '\0') return 0;

    return s1[i] - s2[i];
}

void strcpy(char* dest, const char* src) {
    uint32_t i;

    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];

    dest[i] = '\0';
}

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

uint32_t get_fmt_num_args(const char* fmt) {
    uint32_t count = 0;
    for (uint32_t i = 0; fmt[i] != '\0'; i++) {
        count += (fmt[i] == '%' && fmt[i + 1] == 's');
        count += (fmt[i] == '%' && fmt[i + 1] == 'd');
        count += (fmt[i] == '%' && fmt[i + 1] == 'c');
    }
    return count;
}

void printf(const char* fmt, ...) {
    char printf_buff[PRINTF_PRINT_BUFF_MAX_SIZE];

    va_list arg_list;
    uint32_t num_args = get_fmt_num_args(fmt);
    va_start(arg_list, num_args);

    uint32_t k = 0;
    for (uint32_t i = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i + 1] == 's') {
            char* arg_s = va_arg(arg_list, char*);
            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && arg_s[j] != '\0'; j++)
                printf_buff[k++] = arg_s[j];
            i++;
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'c') {
            printf_buff[k++] = va_arg(arg_list, int);
            i++;
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'd') {
            char temp[ITOA_BUFF_MAX_SIZE];
            itoa(va_arg(arg_list, uint32_t), temp, ITOA_BUFF_MAX_SIZE);

            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && temp[j] != '\0'; j++)
                printf_buff[k++] = temp[j];

            i++;
        }
        else {
            printf_buff[k++] = fmt[i];
        }
    }
    printf_buff[k] = '\0';

    va_end(arg_list);

    puts(printf_buff);
}

char getchar() {
    return sys_get_character_pressed();
}

uint32_t atoi(const char* s) {
    uint32_t out = 0;

    for (uint32_t i = 0; s[i] != '\0'; i++)
        out = out * 10 + (s[i] - '0');

    return out;
}

void scanf(const char* fmt, void* ptr) {
    char scan_buff[SCANF_BUFF_MAX_SIZE];

    uint32_t len = sys_read(scan_buff, SCANF_BUFF_MAX_SIZE);
    scan_buff[len - 1] = '\0';

    if (strcmp(fmt, "%d") == 0)
        *((uint32_t*)ptr) = atoi(scan_buff);
    else if (strcmp(fmt, "%s") == 0)
        strcpy((char*)ptr, scan_buff);
    else if (strcmp(fmt, "%c") == 0)
        *((char*)ptr) = scan_buff[0];
}
