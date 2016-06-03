#include <string.h>

size_t strlen (const char *s) {
	const char *a = s;
	const size_t *w;
	for (; (uintptr_t) s % ALIGN; s++)
		if (!*s)
			return s - a;
	for (w = (const size_t *) s; !HASZERO (*w); w++);
	for (s = (const char *) w; *s; s++);
	return s - a;
}
