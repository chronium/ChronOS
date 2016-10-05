#ifndef _KERNEL_FS_INITRD_H
#define _KERNEL_FS_INITRD_H

#include <dirent.h>

#include <kernel/fs/tar.h>
#include <kernel/fs/filesystem.h>
#include <kernel/fs/file.h>
#include <kernel/list.h>

namespace FileSystem {

class InitrdNode {
public:
  InitrdNode (const char *name, mode_t mode):
    name (name), mode (mode) { }
public:
  const char *name;
  mode_t mode;
};

class InitrdDirectory : public InitrdNode {
public:
  InitrdDirectory (const char *name, mode_t mode):
    InitrdNode (name, mode), children (new List<InitrdNode> ()) { }

private:
  List<InitrdNode> *children;
};

class Initrd : public FileSystem {
public:
  Initrd (const char *path):
    FileSystem (path), nodes (new List<InitrdNode> ()) { }

  void Mount ();
  File *Open (const char *path, int flags);
  struct dirent *ReadDir (DIR *dir);
  int MkDir (const char *path, mode_t mode);
private:
  List<InitrdNode> *nodes;
};

}

#endif
