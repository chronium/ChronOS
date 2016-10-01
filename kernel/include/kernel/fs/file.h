#ifndef _KERNEL_FS_FILE_H
#define _KERNEL_FS_FILESYSTEM_H

namespace FileSystem {

class File {
public:
  inline char *GetName () const { return this->name; };
private:
  char *name;
};

}

#endif
