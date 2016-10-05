#ifndef _KERNEL_FS_FILE_H
#define _KERNEL_FS_FILE_H

#include <stdio.h>
#include <sys/stat.h>

#include <kernel/fs/node.h>
#include <kernel/fs/filesystem.h>

namespace FileSystem {

class File: public Node {
public:
  File (const char *name, mode_t mode, int flags, FileSystem *fs):
    Node (name, mode, fs), open (true), flags (flags) { }

  inline void Close () { this->open = false;};
  inline int Stat (struct stat *buf) {
    buf->st_mode = this->mode;

    return 0;
  };

private:
  bool open;

  int flags;
};

}

#endif
