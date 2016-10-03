#include <string.h>

char *strcpy (char *dest, const char *src) {
    stpcpy (dest, src);
    return dest;
}
