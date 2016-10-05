#include <stdio.h>

#include <kernel/fs/vfs.h>
#include <kernel/fs/mountpoint.h>
#include <kernel/fs/file.h>

#include <sys/stat.h>

#include <kernel/libc.h>

using namespace FileSystem;

namespace VFS {

MountPoint *vfs_root;
List<File> *files;

void InitVFS (MountPoint *root) {
  vfs_root = root;
  vfs_root->Mount ();
  files = new List<File> ();
}

void Mount (MountPoint *mnt) {
  vfs_root->Mount (mnt);
}

int Stat (const char *path, struct stat *buf) {
  return vfs_root->Stat (path, buf);
}

int Open (const char *path, int flags) {
  File *file = vfs_root->Open (path, flags);
  if (file != nullptr)
    return files->add (file);
  return -1;
}

int MkDir (const char *path, mode_t mode) {
  return vfs_root->MkDir (path, mode);
}

struct dirent *ReadDir (DIR *dir) {
  return vfs_root->ReadDir (dir);
}

}

int kstat (const char *path, struct stat *buf) {
  return VFS::Stat (path, buf);
}

int kopen (const char *path, int flags) {
  return VFS::Open (path, flags);
}

int kmkdir (const char *path, mode_t mode) {
  return VFS::MkDir (path, mode);
}

DIR *kfdopendir (int fd) {
  return new DIR { .fd = fd, .filepos = 0 };
} 

struct dirent *kreaddir (DIR *dir) {
  return VFS::ReadDir (dir);
}
