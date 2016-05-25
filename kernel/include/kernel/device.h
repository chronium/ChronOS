#ifndef _KERNEL_DEVICE_H
#define _KERNEL_DEVICE_H

#include <stdint.h>
#include <stddef.h>

typedef enum device_type {
  UNKNOWN_DEV = 0,
  BLOCK_DEV = 1,
  CHAR_DEV = 2,
} dev_type_t;

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
