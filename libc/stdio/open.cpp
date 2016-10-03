#include <fcntl.h>

#if defined (__is_chronos_kernel)
#include <kernel/libc.h>
#endif

int open (const char *path, int flags) {
#if defined (__is_chronos_kernel)
  return kopen (path, flags);
#else
	// TODO: Implement syscall.
#endif
    return -1;
}
