#ifndef _KERNEL_FS_TAR_H
#define _KERNEL_FS_TAR_H

#include <kernel/list.h>
#include <kernel/ramdisk.h>

#include <stddef.h>

namespace FileSystem {

typedef struct tar_header {
  char filename[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[8];
  char typeflag[1];
} tar_header_t;

typedef struct tar_file {
  tar_header_t *header;
  void *content;
} tar_file_t;

class Tar {
public:
  enum FileType {
    RegType,
    ARegType,
    LinkType,
    SymType,
    ChrType,
    BlkType,
    DirType,
    FifoType,
    ContType,
    Unknown
  };

  Tar () { this->headers = new List<tar_file_t> (); };
  ~Tar () { delete this->headers; };

  List<tar_file_t> *GetHeaders () const { return this->headers; };

  static size_t GetSize (tar_file_t *file);
  static size_t GetSize (tar_header_t *header);
  static Tar *Parse (Driver::Ramdisk *ramdisk);
  static FileType GetType (tar_file_t *file);
private:
  List<tar_file_t> *headers;
};

}

#endif
