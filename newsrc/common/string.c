#include <common/stdint.h>

size_t strlen(const char* str){
	register const char *s;

	for (s = str; *s; ++s);
	return s - str;
}

int strnlen(const char* s, size_t maxlen){
    size_t length = 0;
    while (*s++ != '\0' && length != maxlen) length++;
    return length;
}