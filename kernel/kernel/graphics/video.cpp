#include <kernel/device.h>
#include <kernel/video.h>

#if defined (__i386__)
#include <arch/i386/vga.h>
#include <kernel/tty.h>
#endif

struct video *video_inst;
struct video *textscreen;

void init_video (bool graphics) {
  #if defined (__i386__)
  textscreen = init_textmode ();

  if (graphics) {
    video_inst = init_mode13h ();
  }

  #endif
}
