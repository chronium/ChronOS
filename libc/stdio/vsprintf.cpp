#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

static void print_dec (unsigned int value, unsigned int width, char *buf, int *ptr) {
  unsigned int n_width = 1;
  unsigned int i = 9;

  while (value > i && i < UINT32_MAX) {
    n_width += 1;
    i *= 10;
    i += 9;
  }

  int printed = 0;
  while (n_width + printed < width) {
    buf[*ptr] = '0';
    *ptr += 1;
    printed += 1;
  }

  i = n_width;
  while (i > 0) {
    unsigned int n = value / 10;
    int r = value % 10;
    buf[*ptr + i - 1] = r + '0';
    i--;
    value = n;
  }
  *ptr += n_width;
}

static void print_hex (unsigned int value, unsigned int width, char *buf, int *ptr) {
  int i = width;

  if (i == 0) i = 8;

  unsigned int n_width = 1;
  unsigned int j = 0x0F;

  while (value > j && j < UINT32_MAX) {
    n_width += 1;
    j *= 0x10;
    j += 0x0F;
  }

  while (i > (int) n_width) {
    buf[*ptr] = '0';
    *ptr += 1;
    i--;
  }

  i = (int) n_width;
  while (i-- > 0) {
    buf[*ptr] = "0123456789abcdef"[(value >> (i * 4)) & 0x0F];
    *ptr += 1;
  }
}

int vsprintf (char *buf, const char * __restrict format, va_list args) {
  int i = 0;
  char *s;
  char *b = buf;

  for (const char *f = format; *f; f++) {
    if (*f != '%') {
      *b++ = *f;
      continue;
    }

    ++f;
    unsigned int arg_width = 0;
    while (*f >= '0' && *f <= '9') {
      arg_width *= 10;
      arg_width += *f - '0';
      ++f;
    }

    switch (*f) {
      case 's':
        s = (char *) va_arg (args, char *);
        if (s == NULL)
          s = (char *) "(null)";
        while (*s)
          *b++ = *s++;
        break;
      case 'c':
        *b++ = (char) va_arg (args, int);
        break;
      case 'x':
        i = b - buf;
        print_hex ((unsigned long) va_arg (args, unsigned long), arg_width, buf, &i);
        b = buf + i;
        break;
      case 'd':
        i = b - buf;
        print_dec ((unsigned long) va_arg (args, unsigned long), arg_width, buf, &i);
        b = buf + i;
        break;
      case '%':
        *b++ = '%';
        break;
      default:
        *b++ = *f;
        break;
    }
  }

  *b = '\0';
  return b - buf;
}
