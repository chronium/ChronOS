#ifndef _KERNEL_FS_VFS_H
#define _KERNEL_FS_VFS_H

#include <string.h>
#include <dirent.h>

#include <kernel/fs/mountpoint.h>

using namespace FileSystem;

namespace VFS {

class Node;

extern void InitVFS (MountPoint *root);
extern void Mount (MountPoint *mnt);
extern int Open (const char *path, int flags);
extern int Stat (const char *path, struct stat *buf);
extern int MkDir (const char *path, mode_t mode);

extern struct dirent *ReadDir (DIR *dir);

extern MountPoint *vfs_root;
extern List<File> *files;

}

#endif
