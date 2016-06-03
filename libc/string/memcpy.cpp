#include <string.h>

void *memcpy (void *__restrict dest, const void *__restrict src, size_t n) {
#if defined (__i386__)
	int tmp = 0;
	asm volatile ("rep movsb"
				: "=c" (tmp)
				: "D" (dest), "S" (src), "c" (n)
				: "flags", "memory");
#endif
	return dest;
}
