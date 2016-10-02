#include <kernel/fs/filesystem.h>
#include <kernel/fs/mountpoint.h>

namespace FileSystem {

FileSystem::FileSystem (char *path):
  MountPoint (path) { }

}
