#ifndef _KERNEL_FS_FILESYSTEM_H
#define _KERNEL_FS_FILESYSTEM_H

#include <kernel/fs/mountpoint.h>

namespace FileSystem {

class FileSystem : public MountPoint {
public:
  FileSystem (char *path);
};

}

#endif
