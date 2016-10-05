#ifndef _KERNEL_FS_FILESYSTEM_H
#define _KERNEL_FS_FILESYSTEM_H

#include <kernel/fs/mountpoint.h>
#include <kernel/fs/file.h>

#include <dirent.h>

namespace FileSystem {

class FileSystem : public MountPoint {
public:
  FileSystem (const char *path);

  virtual File *Open (const char *path, int flags) = 0;
  virtual struct dirent *ReadDir (DIR *dir) = 0;
  virtual int MkDir (const char *path, mode_t mode) = 0;
};

}

#endif
