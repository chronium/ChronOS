#include <string.h>
#include <stdlib.h>

char *strdup (const char *s) {
  size_t l = strlen (s);
  return (char *) memcpy (malloc (l + 1), s, l + 1);
}
