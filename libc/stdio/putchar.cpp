#include <stdio.h>

#if defined (__is_chronos_kernel)
#include <kernel/tty.h>

extern textmode_t *get_textmode ();
#endif

int putchar (int ic) {
#if defined (__is_chronos_kernel)
	char c = (char) ic;
	term_write (get_textmode (), &c, sizeof (c));
#else
	// TODO: Implement syscall.
#endif
	return ic;
}
