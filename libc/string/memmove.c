#include <string.h>
#include <limits.h>

void *memmove (void *dest, const void *src, size_t n) {
	char *d = dest;
	const char *s = src;

	if (d == s) 
		return d;

	if (s + n <= d || d + n <= s)
		return memcpy (d, s, n);

	if (d < s) {
		if ((uintptr_t) s % ALIGN == (uintptr_t) d % ALIGN) {
			while ((uintptr_t) d % ALIGN) {
				if (!n--)
					return dest;
				*d++ = *s++;
			}

			for (; n >= ALIGN; n -= ALIGN, d += ALIGN, s += ALIGN)
				*(size_t *) d = * (size_t *) s;
		}

		for (; n; n--) 
			*d++ = *s++;
	} else {
		if ((uintptr_t) s % ALIGN == (uintptr_t)d % ALIGN) {
			while ((uintptr_t) (d + n) % ALIGN) {
				if (!n--)
					return dest;
				d[n] = s[n];
			}

			while (n >= ALIGN) {
				n -= ALIGN;
				*(size_t *) (d + n) = *(size_t *) (s + n);
			}
		}

		while (n) {
			n--;
			d[n] = s[n];
		}
	}

	return dest;
}
