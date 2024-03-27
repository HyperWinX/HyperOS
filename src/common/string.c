#include <common/stdint.h>

int strlen(char* string){
    int length = 0;
    while (*string++ != '\0') length++;
    return length;
}

int strnlen(const char* s, size_t maxlen){
    size_t length = 0;
    while (*s++ != '\0' && length != maxlen) length++;
    return length;
}