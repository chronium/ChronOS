#include <string.h>

char *stpcpy (char *d, const char *s) {
    size_t *wd;
    const size_t *ws;

    if ((uintptr_t) s % ALIGN == (uintptr_t)d % ALIGN) {
        for (; (uintptr_t) s % ALIGN; s++, d++) {
            if (!(*d == *s)) {
                return d;
            }
        }

        wd = (size_t *) d;
        ws = (const size_t *) s;
        for (; !HASZERO (*ws); *wd++ = *ws++);
        d = (char *) wd;
        s = (const char *) ws;
    }

    for (; (*d = *s); s++, d++);

    return d;
}
