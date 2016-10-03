#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

#include <kernel/fs/initrd.h>
#include <kernel/fs/file.h>

namespace FileSystem {

void Initrd::Mount () {
  puts ("Initrd mounted");
}

File *Initrd::Open (const char *path, int flags) {
  for (int i = 0; i < 5; i++) {
    tar_file_t *file = this->tarfs->GetHeaders ()->get (i);

    if (!(strncmp(file->header->filename, path, strlen (path)))) {
      mode_t mode;
      switch (Tar::GetType (file)) {
        case Tar::FileType::RegType:
        case Tar::FileType::ARegType:
          mode = __S_IFREG;
          break;
        case Tar::FileType::LinkType:
          mode = __S_IFLNK;
          break;
        case Tar::FileType::SymType:
          mode = __S_IFLNK;
          break;
        case Tar::FileType::ChrType:
          mode = __S_IFCHR;
          break;
        case Tar::FileType::BlkType:
          mode = __S_IFBLK;
          break;
        case Tar::FileType::DirType:
          mode = __S_IFDIR;
          break;
        case Tar::FileType::FifoType:
          mode = __S_IFIFO;
          break;
        case Tar::FileType::ContType:
        case Tar::FileType::Unknown:
          mode = 0;
          break;
      }

      return new File (path, mode, flags);
    }
  }
  return nullptr;
}

}
