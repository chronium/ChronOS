#ifndef _KERNEL_FS_FILE_H
#define _KERNEL_FS_FILE_H

#include <sys/stat.h>

namespace FileSystem {

class File {
public:
  File (char *name, mode_t mode):
    name (name), open (true), mode (mode) { }

  inline char *GetName () const { return this->name; };

  inline void Close () { this->open = false;};
  inline int Stat (struct stat *buf) {
    buf->st_mode = this->mode;

    return 0;
  };

private:
  char *name;

  bool open;
  mode_t mode;
};

}

#endif
