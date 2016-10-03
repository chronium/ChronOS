#include <string.h>

char *strchrnul (const char *s, int c) {
    size_t *w;
    size_t k;

    c = (unsigned char) c;
    if (!c) {
        return (char *)s + strlen (s);
    }

    for (; (uintptr_t) s % ALIGN; s++)
        if (!*s || *(unsigned char *) s == c) {
            return (char *) s;
        }

    k = ONES * c;
    for (w = (size_t *) s; !HASZERO (*w) && !HASZERO (*w^k); w++);
    for (s = (char *) w; *s && *(unsigned char *) s != c; s++);
    return (char *) s;
}
