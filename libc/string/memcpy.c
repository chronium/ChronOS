#include <string.h>

void *memcpy (void *__restrict dest, const void *__restrict src, size_t n) {
#if defined (__i386__)
	asm volatile ("rep movsb"
				: "=c" ((int) { 0 })
				: "D" (dest), "S" (src), "c" (n)
				: "flags", "memory");
#endif
	return dest;
}
