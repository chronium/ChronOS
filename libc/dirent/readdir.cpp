#include <dirent.h>

#if defined (__is_chronos_kernel)
#include <kernel/libc.h>
#endif

struct dirent *readdir (DIR *dir) {
    if (dir == nullptr)
        return nullptr;

#if defined (__is_chronos_kernel)
    return kreaddir (dir);
#else
	// TODO: Implement syscall.
#endif

    return nullptr;
}
