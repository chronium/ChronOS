#ifndef _KERNEL_FS_INITRD_H
#define _KERNEL_FS_INITRD_H

#include <kernel/fs/tar.h>
#include <kernel/fs/filesystem.h>
#include <kernel/fs/file.h>

namespace FileSystem {

class Initrd : public FileSystem {
public:
  Initrd (char *path, Tar *tar):
    FileSystem (path), tarfs (tar) { }

  void Mount ();
  File *Open (const char *path, int flags);

private:
  Tar *tarfs;
};

}

#endif
