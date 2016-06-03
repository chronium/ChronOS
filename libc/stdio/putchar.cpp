#include <stdio.h>

#if defined (__is_chronos_kernel)
#include <kernel/tty.h>
#endif

int putchar (int ic) {
#if defined (__is_chronos_kernel)
	Driver::terminal->PutC ((char) ic);
#else
	// TODO: Implement syscall.
#endif
	return ic;
}
