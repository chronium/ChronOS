#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/stat.h>

#include <kernel/fs/mountpoint.h>
#include <kernel/fs/file.h>
#include <kernel/fs/filesystem.h>

namespace FileSystem {
void MountPoint::Mount (MountPoint *mnt) {
  this->children->add ((FileSystem *) mnt);
  mnt->Mount ();
}

File *MountPoint::Open (const char *path, int flags) {
  File *file = nullptr;
  if (strcmp (path, "/") == 0)
    file = new File ((FileSystem *) this, strdup (path), __S_IFDIR, flags);
  else {
    char *rel = nullptr;
    file = this->FindFileSystem (path, &rel)->Open (rel, flags);
  }

  if (file != nullptr) {
    struct _file *_file = new struct _file;
    _file->file = file;
    _file->path = path;
    this->files->add(_file);
  }

  return file;
}

FileSystem *MountPoint::FindFileSystem (const char *path, char **rel) {
  if (!strncmp (this->path, path, strlen (this->path))) {
    *rel = strdup(MountPoint::RemoveLeadingSlash(path));
    return (FileSystem *) this;
  }
  return nullptr;
  (void) rel;
}

MountPoint *MountPoint::FindMountPoint (const char *path) {
  if (!strncmp (this->path, path, strlen (this->path)))
    return this;
  return nullptr;
}

int MountPoint::Stat (const char *path, struct stat *buf) {
  for (int i = 0; i < this->files->Count (); i++) {
    struct _file *_file = this->files->get (i);

    if (!strncmp (_file->path, path, strlen (_file->path)))
      return _file->file->Stat (buf);
  }

  if (this->Open (path, O_RDONLY) != nullptr)
    return this->Stat (path, buf);

//TODO: ^ Close file when that's implemented

  return -1;
}

int MountPoint::MkDir (const char *path, mode_t mode) {
  char *rel = nullptr;
  return this->FindFileSystem (path, &rel)->MkDir (rel, mode);
}

struct dirent *MountPoint::ReadDir (DIR *dir) {
  File *file = this->files->get (_DIR_dirfd (dir))->file;
  dir->filepos++;

  return file->GetFileSystem ()->ReadDir (dir);
}

}
