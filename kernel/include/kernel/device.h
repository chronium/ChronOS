#ifndef _KERNEL_DEVICE_H
#define _KERNEL_DEVICE_H

#include <stdint.h>
#include <stddef.h>

typedef enum device_type {
  UNKNOWN_DEV = 0,
  BLOCK_DEV = 1,
  CHAR_DEV = 2,
} dev_type_t;

namespace Driver {

template <typename T>
class Device {
public:
  enum class DeviceType {
    UnknownDevice = 0,
    BlockDevice = 1,
    CharDevice = 2,
  };

  Device (int id, const char *name, DeviceType type);
  virtual ~Device ();

  virtual size_t Read (T *buffer, size_t len, uint32_t address) {
    (void) buffer;
    (void) len;
    (void) address;

    return 0;
  }
  virtual size_t Write (const T *buffer, size_t len, uint32_t address) {
    (void) buffer;
    (void) len;
    (void) address;

    return 0;
  }

  inline int GetID () const { return this->id; }
  inline const char *GetName () const { return this->name; }
  inline DeviceType GetType () const { return this->type; }
private:
  int id;
  const char *name;
  DeviceType type;
};

}

struct device {
  char           *dev_name;
  dev_type_t      dev_type;
  int             dev_id;
  void           *dev_tag;
  size_t        (*read)   (void *, void *, size_t, uint32_t);
  size_t        (*write)  (void *, const void *, size_t, uint32_t);
  struct device  *next;
};

struct device *dev_create (dev_type_t, const char *);

size_t dev_read   (struct device *, void *, size_t, uint32_t);
size_t dev_write  (struct device *, const void *, size_t, uint32_t);

#endif
