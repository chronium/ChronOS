#ifndef _DIRENT_H
#define _DIRENT_H 1

#include <dirstream.h>
#include <bits/dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __dirstream DIR;

DIR *opendir (const char *name);
DIR *fdopendir (int __fd);

struct dirent *readdir (DIR *dir);

#ifdef __cplusplus
}
#endif

#endif
