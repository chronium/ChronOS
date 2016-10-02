#ifndef _KERNEL_LIBC_H
#define _KERNEL_LIBC_H

#include <sys/stat.h>

extern "C" int kstat (const char *path, struct stat *buf);

#endif
