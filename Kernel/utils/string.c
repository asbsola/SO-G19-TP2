// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils/string.h>

uint64_t str_len(const char *str) {
	uint64_t len;
	for (len = 0; str[len] != '\0'; len++)
		;
	return len;
}

void str_cpy(char *dest, const char *src) {
	uint64_t i;
	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];

	dest[i] = '\0';
}

int str_cmp(const char *s1, const char *s2) {
	int i;
	for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
		if (s1[i] != s2[i])
			return s1[i] - s2[i];

	if (s1[i] == '\0' && s2[i] == '\0')
		return 0;

	return s1[i] - s2[i];
}
