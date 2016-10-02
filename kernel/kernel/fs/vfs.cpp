#include <stdio.h>

#include <kernel/fs/vfs.h>
#include <kernel/fs/mountpoint.h>

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

int Open (const char *path) {
  File *file = vfs_root->Open (path);
  if (file != nullptr)
    return files->add (file);
  return -1;
}

}

int kstat (const char *path, struct stat *buf) {
  return VFS::Stat (path, buf);
}
