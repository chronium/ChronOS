#ifndef _KERNEL_FS_MOUNTPOINT_H
#define _KERNEL_FS_MOUNTPOINT_H

#include <sys/stat.h>

#include <dirent.h>
#include <string.h>

#include <kernel/list.h>

#include <kernel/fs/node.h>

namespace FileSystem {

class File;
class FileSystem;

struct _file {
  File *file;
  const char *path;
};

class MountPoint: public Node {
public:
  MountPoint (const char *path);

  File *Open (const char *path, int flags);

  struct dirent *ReadDir (DIR *dir);

  int Stat (const char *path, struct stat *buf);
  int MkDir (const char *path, mode_t mode);

  MountPoint *FindMountPoint (const char *path);
  FileSystem *FindFileSystem (const char *path, char **rel);

  inline const char *RemoveLeadingSlash (const char *s) { return *s == '/' ? ++s : s; };
  inline const char *RemoveLeastSlash (const char *s) {
    if (s[strlen (s) - 1] == '/') {
      char *ret = strdup (s);
      ret[strlen (s) - 1] = '\0';
      return ret;
    }

    return s;
  }

  void Mount (MountPoint *mnt);
  virtual void Mount () { }

private:
  List<FileSystem> *children;
  List<struct _file> *files;
};

}

#endif
