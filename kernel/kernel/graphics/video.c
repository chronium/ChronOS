#include <kernel/device.h>
#include <kernel/video.h>

#if defined (__i386__)
#include <arch/i386/vga.h>
#endif

void init_video (bool graphics) {
  #if defined (__i386__)
  textscreen = init_textmode ();

  if (graphics) {
    video = init_mode13h ();
  }

  #endif
}
