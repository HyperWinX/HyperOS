#include <common/stdint.h>

void memset(void* dst, uint8_t data, uint32_t size) {
	if (size % 4 == 0) while (size -= 4) *(uint32_t*)dst++ = (uint32_t)data;
	else if (size % 2 == 0) while (size -= 2) *(uint16_t*)dst++ = (uint16_t)data;
	else while (size--) *(uint8_t*)dst++ = data;
}