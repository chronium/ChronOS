#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

#include <kernel/fs/mountpoint.h>
#include <kernel/fs/file.h>
#include <kernel/fs/filesystem.h>

namespace FileSystem {
void MountPoint::Mount (MountPoint *mnt) {
  this->children->add ((FileSystem *) mnt);
  mnt->Mount ();
}

File *MountPoint::Open (const char *path) {
  File *file = nullptr;
  if (strcmp (path, "/") == 0)
    file = new File (strdup (path), __S_IFDIR);

  if (file != nullptr) {
    struct _file *_file = new struct _file;
    _file->file = file;
    _file->path = path;
    this->files->add(_file);
  }

  return file;
}

int MountPoint::Stat (const char *path, struct stat *buf) {
  for (int i = 0; i < this->files->Count (); i++) {
    struct _file *_file = this->files->get (i);

    if (strcmp (_file->path, path) == 0)
      return _file->file->Stat (buf);
  }

  return -1;
}

}
