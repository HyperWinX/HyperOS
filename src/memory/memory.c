
extern void _memcpy_asm(unsigned int dest, unsigned int src, unsigned int size); // see stdlib.asm
void memcpy(unsigned int dest, unsigned int src, unsigned int size) {
	_memcpy_asm(dest, src, size);
}

extern void _memset_asm(unsigned int addr, unsigned char fill, unsigned int size); // see stdlib.asm
void memset(void* dst, unsigned char data, unsigned int size) {
	if (size % 4 == 0) while (size -= 4) *(unsigned int*)dst++ = (unsigned int)data;
	else if (size % 2 == 0) while (size -= 2) *(unsigned short*)dst++ = (unsigned short)data;
	else while (size--) *(unsigned char*)dst++ = data;
}