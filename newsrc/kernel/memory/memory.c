#include <common/stdint.h>

void memset(void* _Nonnull restrict dst, const uint8_t data, size_t size) {
	if (size % 4 == 0) while (size -= 4) *(uint32_t*)dst++ = (uint32_t)data;
	else if (size % 2 == 0) while (size -= 2) *(uint16_t*)dst++ = (uint16_t)data;
	else while (size--) *(uint8_t*)dst++ = data;
}

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}