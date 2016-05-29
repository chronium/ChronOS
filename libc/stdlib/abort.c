#include <stdlib.h>
#include <stdio.h>

__attribute__ ((__noreturn__))
void abort (void) {
#if __STDC_HOSTED__
	// TODO: Implement hosted abort ().=
#elif defined (__is_chronos_kernel)
	// TODO: Implement better kernel panic.
	printf ("Kernel Panic: abort ()\n");
	exit (1);
#else
	#error "You need to implement abort() in the current freestanding environment."
#endif
	__builtin_unreachable ();
}
