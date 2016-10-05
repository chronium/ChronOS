#ifndef _KERNEL_FS_DEVFS_H
#define _KERNEL_FS_DEVFS_H

#include <kernel/fs/filesystem.h>

namespace FileSystem {

class DevFS: public FileSystem {
public:
    DevFS ():
        FileSystem ("/devfs") { }
};

}

#endif
