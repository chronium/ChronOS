#ifndef _KERNEL_FS_INITRD_H
#define _KERNEL_FS_INITRD_H

#include <kernel/fs/tar.h>
#include <kernel/fs/filesystem.h>

namespace FileSystem {

class Initrd : public FileSystem {
public:
  Initrd (char *path, Tar *tar):
    FileSystem (path), tarfs (tar) { }

  void Mount ();

private:
  Tar *tarfs;
};

}

#endif
