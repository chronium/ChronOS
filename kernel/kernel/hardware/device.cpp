#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/device.h>

struct device *dev_list = NULL;
static int next_devid = 0;

struct device *dev_create (dev_type_t type, const char *name) {
  struct device *dev = (struct device *) malloc (sizeof (struct device));

  dev->dev_name = strdup (name);
  dev->next = NULL;
  dev->dev_id = next_devid++;
  dev->dev_type = type;

  struct device *i = dev_list;

  if (!dev_list)
    dev_list = dev;
  else {
    while (i->next)
      i = i->next;
    i->next = dev;
  }

  return dev;
}

size_t dev_read (struct device *dev, void *buff, size_t size, uint32_t addr) {
  return dev->read (dev->dev_tag, buff, size, addr);
}

size_t dev_write (struct device *dev, const void *buff, size_t size, uint32_t addr) {
  return dev->write (dev->dev_tag, buff, size, addr);
}
