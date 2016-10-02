#include <stdio.h>

#include <kernel/fs/initrd.h>

namespace FileSystem {

void Initrd::Mount () {
  puts ("Initrd mounted");
}

}
