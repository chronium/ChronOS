#ifndef _KERNEL_FS_INITRD_H
#define _KERNEL_FS_INITRD_H

#include <dirent.h>

#include <kernel/fs/tar.h>
#include <kernel/fs/filesystem.h>
#include <kernel/fs/file.h>
#include <kernel/fs/node.h>
#include <kernel/list.h>

namespace FileSystem {

class Initrd : public FileSystem {
public:
  Initrd (const char *path);

  void Mount ();
  Node *Open (const char *path, int flags);
  struct dirent *ReadDir (DIR *dir);
  int MkDir (const char *path, mode_t mode);
private:
  List<Node> *nodes;
};

}

#endif
