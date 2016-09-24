#ifndef _KERNEL_VGA_CONTEXT_H
#define _KERNEL_VGA_CONTEXT_H

#include <kernel/context.h>
#include <kernel/video.h>

class VGAContext : public Context {
public:
  VGAContext (struct video *vga);

  inline void swap_buff () { swap_buffers (vga, 0, 0, 320, 200); };
private:
  struct video *vga;
};

#endif
