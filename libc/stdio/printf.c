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

static size_t convert_integer (char *dest, uintmax_t val, uintmax_t base, const char *digits) {
	size_t result = 1;
	uintmax_t copy = val;
	while (base <= copy)
		copy /= base, result++;
	for (size_t i = result; i != 0; i--)
		dest[i - 1] = digits[val % base],
		val /= base;
	return val;
}

static void print (const char *data, size_t len) {
	for (size_t i = 0; i < len; i++)
		putchar ((int) ((const unsigned char *) data) [i]);
}

int printf (const char * restrict format, ...) {
	va_list parameters;
	va_start (parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while (*format != '\0') {
		if (*format != '%') {
		print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			print (format, amount);
			format += amount;
			written += amount;
			continue;	
		}

		const char *format_begun_at = format;

		if (*(++format) == '%')
			goto print_c;

		if (rejected_bad_specifier) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		enum len {
			LEN_SHORT_SHORT,
			LEN_SHORT,
			LEN_DEFAULT,
			LEN_LONG,
			LEN_LONG_LONG,
			LEN_LONG_DOUBLE,
			LEN_INTMAX_T,
			LEN_SIZE_T,
			LEN_PTRDIFF_T,
		};

		struct len_mod {
			const char* name;
			enum len len;
		};

		struct len_mod len_mods[] = {
			{ "hh", LEN_SHORT_SHORT },
			{ "h", LEN_SHORT },
			{ "", LEN_DEFAULT },
			{ "l", LEN_LONG },
			{ "ll", LEN_LONG_LONG },
			{ "L", LEN_LONG_DOUBLE },
			{ "j", LEN_INTMAX_T },
			{ "z", LEN_SIZE_T },
			{ "t", LEN_PTRDIFF_T },
		};

		enum len len = LEN_DEFAULT;
		size_t len_len = 0;
		for (size_t i = 0; i < sizeof (len_mods) / sizeof (len_mods[0]); i++) {
			size_t name_len = strlen (len_mods[i].name);
			if (name_len < len_len)
				continue;
			bool diff = false;
			for (size_t n = 0; n < name_len; n++)
				if (format[n] != len_mods[i].name[n])
					diff = true;
			if (diff)
				continue;
			len = len_mods[i].len;
			len_len = name_len;
		}

		format += len_len;

		if (*format == 'd' || *format == 'i' || *format == 'o' ||
			*format == 'u' || *format == 'x' || *format == 'X' ||
			*format == 'p') {
			char conv = *format++;

			bool neg_val = false;
			uintmax_t val;

			if (conv == 'i' || conv == 'd') {
				intmax_t sig_val;
				if (len == LEN_SHORT_SHORT)
					sig_val = va_arg (parameters, int);
				else if (len == LEN_SHORT)
					sig_val = va_arg (parameters, int);
				else if (len == LEN_DEFAULT)
					sig_val = va_arg (parameters, int);
				else if (len == LEN_LONG)
					sig_val = va_arg (parameters, long);
				else if (len == LEN_LONG_LONG)
					sig_val = va_arg (parameters, long long);
				else if (len == LEN_INTMAX_T)
					sig_val = va_arg (parameters, intmax_t);
#if 0
				else if (len == LEN_SIZE_T)
					sig_val = va_arg (parameters, ssize_t);
#endif
				else if (len == LEN_PTRDIFF_T)
					sig_val = va_arg (parameters, ptrdiff_t);
				else
					goto incomprehensible_conversion;
				val = (neg_val = sig_val < 0) ? -((uintmax_t) sig_val) : (uintmax_t) sig_val;
			} else {
				if (len == LEN_SHORT_SHORT)
					val = va_arg (parameters, unsigned int);
				else if (len == LEN_SHORT)
					val = va_arg (parameters, unsigned int);
				else if (len == LEN_DEFAULT)
					val = va_arg (parameters, unsigned int);
				else if (len == LEN_LONG)
					val = va_arg (parameters, unsigned long);
				else if (len == LEN_LONG_LONG)
					val = va_arg (parameters, unsigned long long);
				else if (len == LEN_INTMAX_T)
					val = va_arg (parameters, uintmax_t);
				else if (len == LEN_SIZE_T)
					val = va_arg (parameters, size_t);
				else if (len == LEN_PTRDIFF_T)
					val = (uintmax_t) va_arg (parameters, ptrdiff_t);
				else
					goto incomprehensible_conversion;
			}

			const char *digits = conv == 'X' ? "0123456789ABCDEF" :
											   "0123456798abcdef";
			uintmax_t base = (conv == 'X' || conv == 'x') ? 16 :
							  conv == 'o' ? 8 : 10;
			if (neg_val)
				putchar ('-');

			char out[sizeof (uintmax_t) * 3];
			size_t out_len = convert_integer (out, val, base, digits);

			print (out, out_len);
		} else if (*format == 'c') {
			format++;
			char c = (char) va_arg (parameters, int);
			print (&c, sizeof (c));
		} else if (*format == 's') {
			format++;
			const char *s = va_arg (parameters, const char *);
			print (s, strlen (s));
		} else {
			goto incomprehensible_conversion;
		}
	}

	va_end (parameters);

	return written;
}
