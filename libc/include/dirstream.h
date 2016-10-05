#ifndef _DIRSTREAM_H
#define _DIRSTREAM_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct __dirstream {
    int fd;
    int filepos;
};

#define _DIR_dirfd(dirp) ((dirp)->fd)

#ifdef __cplusplus
}
#endif

#endif
