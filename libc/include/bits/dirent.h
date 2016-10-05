#ifndef _BITS_DIRENT_H
#define _BITS_DIRENT_H 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct dirent {
    int ino;
    uint8_t type;
    char name[256];
};

#ifdef __cplusplus
}
#endif

#endif
