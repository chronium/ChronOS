#include <dirent.h>

#if defined (__is_chronos_kernel)
#include <kernel/libc.h>
#endif

DIR *fdopendir (int __fd) {
    if (__fd == -1)
        return nullptr;

#if defined (__is_chronos_kernel)
    return kfdopendir (__fd);
#else
	// TODO: Implement syscall.
#endif

    return nullptr;
}
