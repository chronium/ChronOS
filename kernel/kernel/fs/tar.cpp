#include <kernel/fs/tar.h>

#include <stddef.h>

namespace FileSystem {

size_t Tar::GetSize (tar_header_t *header) {
  size_t size = 0;
  size_t count = 1;

  for (size_t j = 11; j > 0; j--, count *= 8)
    size += (header->size[j - 1] - '0') * count;

  return size;
}

Tar *Tar::Parse (Driver::Ramdisk *ramdisk) {
  auto *tar = new Tar ();

  uint32_t address = 0;

  for (size_t i = 0; ; i++) {
    char *hdr = new char[sizeof (tar_header_t)];
    ramdisk->Read (hdr, sizeof (tar_header_t), address);

    tar_header_t *header = (struct tar_header *) hdr;

    if (header->filename[0] == '\0')
      break;

    tar->headers->insert (header);

    size_t size = Tar::GetSize (header);
    address += ((size / 512) + 1) * 512;

    if (size % 512)
      address += 512;
  }

  return tar;
}

Tar::FileType Tar::GetType (tar_header_t *header) {
  switch (header->typeflag[0]) {
    case '0':
      return FileType::RegType;
    case '\0':
      return FileType::ARegType;
    case '1':
      return FileType::LinkType;
    case '2':
      return FileType::SymType;
    case '3':
      return FileType::ChrType;
    case '4':
      return FileType::BlkType;
    case '5':
      return FileType::DirType;
    case '6':
      return FileType::FifoType;
    case '7':
      return FileType::ContType;
  }

  return FileType::Unknown;
}

}
