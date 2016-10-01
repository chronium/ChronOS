#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/device.h>

namespace Driver {

Device::Device (int id, const char *name, DeviceType type) :
  id (id),
  name (name),
  type (type) { }

Device::~Device () { }

}
