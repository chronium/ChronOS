#include <sys/stat.h>

#if defined (__is_chronos_kernel)
#include <kernel/libc.h>
#endif

int mkdir (const char *path, mode_t mode) {
#if defined (__is_chronos_kernel)
  return kmkdir (path, mode);
#else
	// TODO: Implement syscall.
#endif 
    return -1;
}
