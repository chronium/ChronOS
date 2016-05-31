#include <kernel/device.h>
#include <kernel/video.h>

#if defined (__i386__)
#include <arch/i386/vga.h>
#endif

int init_video (bool textmode) {
  if (textmode) {
    #if defined (__i386__)
    video = init_textmode ();

    return 1;
    #endif
    return 0;
  }

  return 0;
}
