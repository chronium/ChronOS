#include <stdlib.h>
#include <stdbool.h>

__attribute__ ((__noreturn__))
void exit (int code) {
	while (true) {
		// TODO: Implement exit syscall
		(void) code;
	}
}
