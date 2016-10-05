#include <kernel/video.h>

namespace Driver {

Video::Video (const char *name, DeviceType type, size_t width, size_t height, size_t bpe) :
  Device (name, type),
  width (width),
  height (height),
  bpe (bpe / 8) { }

size_t Video::Read (void *buffer, size_t len, uint32_t address) {
  (void) buffer;
  (void) len;
  (void) address;

  return 0;
}

size_t Video::Write (const void *buffer, size_t len, uint32_t address) {
  (void) buffer;
  (void) len;
  (void) address;

  return 0;
}

}
