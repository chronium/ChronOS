#ifndef _KERNEL_RAMDISK_H
#define _KERNEL_RAMDISK_H

#include <kernel/device.h>

namespace Driver {

class Ramdisk : public Device {
public:
  Ramdisk (const char *name, void *memory);
  ~Ramdisk ();

  size_t Read (void *buffer, size_t len, uint32_t address);
  size_t Write (const void *buffer, size_t len, uint32_t address);

private:
  char *memory_ptr;
};

}

#endif
