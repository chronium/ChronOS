#include <kernel/ramdisk>
#include <kernel/device.h>

#include <string.h>

namespace Driver {

Ramdisk::Ramdisk (int id, const char *name, void *memory) :
  Device (id, name, DeviceType::BlockDevice),
  memory_ptr ((char *) memory) { }

Ramdisk::~Ramdisk () { }

size_t Ramdisk::Read (void *buffer, size_t len, uint32_t offset) {
  memcpy (buffer, &this->memory_ptr[offset], len);
  return len;
}

size_t Ramdisk::Write (const void *buffer, size_t len, uint32_t offset) {
  memcpy (&this->memory_ptr[offset], buffer, len);
  return len;
}

}
