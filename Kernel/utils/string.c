#include <utils/string.h>

uint64_t my_strlen(const char* str) {
    uint64_t len;
    for (len = 0; str[len] != '\0'; len++) ;
    return len;
}

void my_strcpy(char* dest, const char* src) {
    for (uint64_t i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
}
