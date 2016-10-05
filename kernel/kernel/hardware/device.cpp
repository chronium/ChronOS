#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/device.h>
#include <kernel/devmanager.h>

namespace Driver {

Device::Device (const char *name, DeviceType type) :
  name (name),
  type (type) { 
  DeviceManager::AddDevice (this);
}

Device::~Device () { }

}
