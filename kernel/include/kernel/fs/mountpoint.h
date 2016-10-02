#ifndef _KERNEL_FS_MOUNTPOINT_H
#define _KERNEL_FS_MOUNTPOINT_H

#include <sys/stat.h>

#include <kernel/list.h>
#include <kernel/fs/file.h>

namespace FileSystem {

struct _file {
  File *file;
  const char *path;
};

class FileSystem;

class MountPoint {
public:
  MountPoint (char *path):
    path (path),
    children (new List<FileSystem> ()),
    files (new List<struct _file> ()) { }

  File *Open (const char *path);

  int Stat (const char *path, struct stat *buf);

  void Mount (MountPoint *mnt);
  virtual void Mount () { }

private:
  char *path;
  List<FileSystem> *children;
  List<struct _file> *files;
};

}

#endif
