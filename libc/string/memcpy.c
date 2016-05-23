#include <string.h>

void *memcpy (void *__restrict dest, const void *__restrict src, size_t n) {
	asm volatile ("rep movsb"
				: "=c" ((int) { 0 })
				: "D" (dest), "S" (src), "c" (n)
				: "flags", "memory");
	return dest;
}
