#ifndef _KERNEL_VIDEO_H
#define _KERNEL_VIDEO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/device.h>

namespace Driver {

class Video : public Device {
public:
  Video (int id, const char *name, DeviceType type, size_t width, size_t height, size_t bpe);
  virtual ~Video () { }

  virtual size_t Read (void *buffer, size_t len, uint32_t address);
  virtual size_t Write (const void *buffer, size_t len, uint32_t address);

  virtual void *GetAddress () const { return 0; }

  inline size_t GetWidth () const { return this->width; }
  inline size_t GetHeight () const { return this->height; }

  inline uint32_t GetPitch () const { return this->width * this->bpe;}
  inline uint32_t GetSize () const { return this->height * this->GetPitch (); }
private:
  size_t width;
  size_t height;

  size_t bpe;
};
}

#endif
