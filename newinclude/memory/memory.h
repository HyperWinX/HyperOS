#include <common/stdint.h>

void* memcpy(void* dest, const void* src, size_t n);
void memset(void* s,
            char c,
            size_t n);
void memmove(void* restrict dst,
             const void* restrict src,
             size_t n);
int memcmp( const void* restrict s1,
            const void* restrict s2,
            size_t n);