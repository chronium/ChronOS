#ifndef _KERNEL_FS_VFS_H
#define _KERNEL_FS_VFS_H

#include <string.h>

#include <kernel/fs/mountpoint.h>

using namespace FileSystem;

namespace VFS {

extern void InitVFS (MountPoint *root);
extern void Mount (MountPoint *mnt);
extern int Open (const char *path);
extern int Stat (const char *path, struct stat *buf);

extern MountPoint *vfs_root;
extern List<File> *files;

}

#endif
