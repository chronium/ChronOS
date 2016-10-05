#ifndef _KERNEL_FS_FILE_H
#define _KERNEL_FS_FILE_H

#include <stdio.h>
#include <sys/stat.h>

namespace FileSystem {

class FileSystem;

class File {
public:
  File (FileSystem *fs, const char *name, mode_t mode, int flags):
    fs(fs), name (name), open (true), mode (mode), flags (flags) { }

  inline const char *GetName () const { return this->name; };

  inline void Close () { this->open = false;};
  inline int Stat (struct stat *buf) {
    buf->st_mode = this->mode;

    return 0;
  };

  inline FileSystem *GetFileSystem () const { return this->fs; };

private:
  FileSystem *fs;

  const char *name;

  bool open;
  mode_t mode;

  int flags;
};

}

#endif
