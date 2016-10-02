#include <sys/stat.h>

#if defined (__is_chronos_kernel)
#include <kernel/libc.h>
#endif

int stat (const char *path, struct stat *buf) {
#if defined (__is_chronos_kernel)
  return kstat (path, buf);
#else
	// TODO: Implement syscall.
#endif
  return -1;
}
