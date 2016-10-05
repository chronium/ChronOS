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
  (void) path;
  (void) flags;

  for (int i = 0; i < this->nodes->Count (); i++) {
    auto *node = this->nodes->get (i);

    if (!strncmp (node->name, path, strlen (node->name))) {
      return new File (this, path, node->mode, flags);
    }
  }

  /*for (int i = 0; i < this->tarfs->GetHeaders ()->Count (); i++) {
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

      return new File (this, path, mode, flags);
    }
  }*/
  return nullptr;
}

struct dirent *Initrd::ReadDir (DIR *dir) {
  (void) dir;
  return nullptr;
}

int Initrd::MkDir (const char *path, mode_t mode) {
  this->nodes->add (new InitrdDirectory (RemodeLeastSlash(path), __S_IFDIR | mode));

  return -1;
}

}
