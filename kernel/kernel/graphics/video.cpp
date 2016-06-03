#include <kernel/video.h>

#if defined (__i386__)
#include <arch/i386/vga.h>
#endif

namespace Driver {

Video::Video (int id, const char *name, DeviceType type, size_t width, size_t height, size_t bpe) :
  Device (id, name, type),
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

struct video *video_inst;

void init_video (bool graphics) {
  #if defined (__i386__)

  if (graphics) {
    video_inst = init_mode13h ();
  }

  #endif
}
