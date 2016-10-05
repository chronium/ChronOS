#include <kernel/fs/filesystem.h>
#include <kernel/fs/mountpoint.h>

namespace FileSystem {

FileSystem::FileSystem (const char *path):
  MountPoint (path) { }

}
