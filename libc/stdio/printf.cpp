#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#if __STDC_HOSTED__
#include <stdint-gcc.h>
#else
#include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>

static void print (const char *data, size_t len) {
	for (size_t i = 0; i < len; i++)
		putchar ((int) ((const unsigned char *) data) [i]);
}

int printf (const char * __restrict format, ...) {
	va_list argp;
	va_start (argp, format);

	char tmp_buff[512];
	memset (tmp_buff, 0, 512);

	int tmp = vsprintf (tmp_buff, format, argp);
	va_end (argp);

	print (tmp_buff, tmp);

	return tmp;
}
