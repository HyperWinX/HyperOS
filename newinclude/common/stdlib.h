#ifndef STDLIB_H
#define STDLIB_H

#include <common/stdbool.h>
#include <common/stdint.h>

int32_t litoa(int64_t num, char* str);
int32_t itoa(int32_t integer, char* buffer);
int32_t itoah(int32_t num, char* buffer);
int32_t itoab(int32_t num, char* buffer);

bool strcmp(char* string1, char* string2);
bool safe_strcmp(char* string1, int str1len, char* string2, int str2len);

#endif