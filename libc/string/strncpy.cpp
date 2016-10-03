#include <string.h>

char *strncpy (char *dest, const char *src, size_t n) {
    char *s = dest;
    while (n > 0 && *src != '\0') {
        *s++ = *src++;
        --n;
    }
    while (n > 0) {
        *s++ = '\0';
        --n;
    }
    return dest;
}
