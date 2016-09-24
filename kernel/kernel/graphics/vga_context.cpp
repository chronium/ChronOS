#include <kernel/vga_context.h>

VGAContext::VGAContext (struct video *vga) :
  Context (vga->buffer, (uint16_t) vga->width, (uint16_t) vga->height) {
  this->vga = vga;
}
