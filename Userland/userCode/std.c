// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <std.h>

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

void strcat(char* dest, const char* str1, const char * str2) {
    uint32_t i;
    uint32_t j;

    for (i = 0; str1[i] != '\0'; i++)
        dest[i] = str1[i];

    for (j = 0; str2[j] != '\0'; j++)
        dest[i + j] = str2[j];

    dest[i + j] = '\0';
}

uint32_t strlen(const char* s) {
    uint32_t i;

    for (i = 0; s[i] != '\0'; i++) ;

    return i;
}

void putchar(char c) {
    sys_write(sys_get_stdout(), &c, 1);
}

void puts(const char* s) {
    sys_write(sys_get_stdout(), s, strlen(s));
}

char* num_to_color_hex_string(uint32_t hexColor, char* buff) {
    const char hex_digits[] = "0123456789abcdef";

    for (int i = 0; i < HEX_COLOR_STRING_LEN; i++) {
        buff[HEX_COLOR_STRING_LEN - i - 1] = hex_digits[hexColor % 16]; 
        hexColor /= 16;
    }

    buff[HEX_COLOR_STRING_LEN] = '\0';
    return buff;
}

void puts_with_color(const char* s, uint32_t hexColor) {
    uint32_t len = strlen(s);

    char puts_buffer[len + HEX_COLOR_STRING_LEN + 2];
    puts_buffer[0] = '\e';
    num_to_color_hex_string(hexColor, &puts_buffer[1]);

    strcpy(&puts_buffer[HEX_COLOR_STRING_LEN + 1], s);

    sys_write(sys_get_stdout(), puts_buffer, len + HEX_COLOR_STRING_LEN + 1);
}

char itoa_buff[ITOA_BUFF_MAX_SIZE] = {0};

char* itoa(int64_t num, char* dest, uint32_t dest_max_len) {
    itoa_buff[0] = 0; 

    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num; 
    }

    uint32_t i;
    for (i = 0; num != 0; num /= 10)
        itoa_buff[i++] = num % 10;

    if (i == 0) i = 1;
    if(is_negative) i++;

    uint32_t j;
    for (j = 0; j < dest_max_len && j < i; j++)
        dest[j] = itoa_buff[i - j - 1] + '0';

    dest[j] = '\0';
    if(is_negative) dest[0] = '-';

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

//-V::V576
void printf(const char* fmt, ...) {
    char printf_buff[PRINTF_PRINT_BUFF_MAX_SIZE] = {0};

    va_list arg_list;
    va_start(arg_list, fmt);

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
            itoa(va_arg(arg_list, int32_t), temp, ITOA_BUFF_MAX_SIZE);

            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && temp[j] != '\0'; j++)
                printf_buff[k++] = temp[j];

            i++;
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'd') {
            char temp[ITOA_BUFF_MAX_SIZE];
            itoa(va_arg(arg_list, int64_t), temp, ITOA_BUFF_MAX_SIZE);

            for (uint64_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && temp[j] != '\0'; j++)
                printf_buff[k++] = temp[j];

            i += 2;
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
//-V::scanf
void scanf(const char* fmt, void* ptr) {
    char scan_buff[SCANF_BUFF_MAX_SIZE] = {0};

    uint32_t len = sys_read(sys_get_stdin(), scan_buff, SCANF_BUFF_MAX_SIZE);
    scan_buff[len - 1] = '\0';

    if (strcmp(fmt, "%d") == 0)
        *((uint32_t*)ptr) = atoi(scan_buff);
    else if (strcmp(fmt, "%s") == 0)
        strcpy((char*)ptr, scan_buff);
    else if (strcmp(fmt, "%c") == 0)
        *((char*)ptr) = scan_buff[0];
}

int isAlpha(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

int isVowel(char c){
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

void sleep(uint64_t millis){
    uint64_t start = sys_get_ticks();
    while((sys_get_ticks() - start) * 1000 < (PIT_FREQUENCY * millis));
}
