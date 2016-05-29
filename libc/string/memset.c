#include <string.h>

void *memset (void *dest, int c, size_t n) {
#if defined (__i386__)
	asm volatile ("rep stosb"
				: "=c" ((int) { 0 })
				: "D" (dest), "a" (c), "c" (n)
				: "flags", "memory");
#endif
	return dest;
}
