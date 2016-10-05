#ifndef _KERNEL_DEVICE_H
#define _KERNEL_DEVICE_H

#include <stdint.h>
#include <stddef.h>

namespace Driver {

class Device {
public:
  enum class DeviceType {
    UnknownDevice = 0,
    BlockDevice = 1,
    CharDevice = 2,
  };

  Device (const char *name, DeviceType type);
  virtual ~Device ();

  virtual size_t Read (void *buffer, size_t len, uint32_t address) {
    (void) buffer;
    (void) len;
    (void) address;

    return 0;
  }
  virtual size_t Write (const void *buffer, size_t len, uint32_t address) {
    (void) buffer;
    (void) len;
    (void) address;

    return 0;
  }

  inline const char *GetName () const { return this->name; }
  inline DeviceType GetType () const { return this->type; }
private:
  const char *name;
  DeviceType type;
};

}

#endif
