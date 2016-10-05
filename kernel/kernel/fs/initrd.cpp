#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

#include <kernel/fs/initrd.h>
#include <kernel/fs/file.h>
#include <kernel/fs/node.h>
#include <kernel/fs/directory.h>

namespace FileSystem {

Initrd::Initrd (const char *path):
    FileSystem (path), nodes (new List<Node> ()) { }

void Initrd::Mount () {
  puts ("Initrd mounted");
}

Node *Initrd::Open (const char *path, int flags) {
  for (int i = 0; i < this->nodes->Count (); i++) {
    auto *node = this->nodes->get (i);

    if (!strncmp (node->name, path, strlen (node->name))) {
      return new File (path, node->mode, flags, this);
    }
  }

  return nullptr;
}

struct dirent *Initrd::ReadDir (DIR *dir) {
  (void) dir;
  return nullptr;
}

int Initrd::MkDir (const char *path, mode_t mode) {
  this->nodes->add (new Directory (RemoveLeastSlash(path), __S_IFDIR | mode, this));

  return -1;
}

}
