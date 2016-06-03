#include <string.h>

void *memset (void *dest, int c, size_t n) {
#if defined (__i386__)
	int tmp = 0;
	asm volatile ("rep stosb"
				: "=c" (tmp)
				: "D" (dest), "a" (c), "c" (n)
				: "flags", "memory");
#endif
	return dest;
}
