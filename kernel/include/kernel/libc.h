#ifndef _KERNEL_LIBC_H
#define _KERNEL_LIBC_H

#include <sys/stat.h>
#include <dirent.h>

extern "C" int kstat (const char *path, struct stat *buf);
extern "C" int kopen (const char *path, int flags);

extern "C" int kmkdir (const char *path, mode_t mode);

extern "C" DIR *kfdopendir (int fd);
extern "C" struct dirent *kreaddir (DIR *dir);

#endif
