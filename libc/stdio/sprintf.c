#include <stdarg.h>
#include <stdio.h>

int sprintf (char *dest, const char * restrict format, ...) {
  va_list argp;

  va_start (argp, format);
  int temp = vsprintf (dest, format, argp);
  va_end (argp);
  return temp;
}
