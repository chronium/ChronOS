#include <kernel/context>
#include <kernel/video.h>

Context::Context (struct video *vga) {
  this->width = (uint16_t) vga->width;
  this->height = (uint16_t) vga->height;

  this->buffer = vga->buffer;
  this->vga = vga;
}

void Context::fill_rect (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  uint16_t cur_x;
  uint16_t cur_y;
  uint16_t max_x = x + width;
  uint16_t max_y = y + height;

  if (max_x > this->width)
    max_x = this->width;
  if (max_y > this->height)
    max_y = this->height;

  for (cur_y = y; cur_y < max_y; cur_y++)
    for (cur_x = x; cur_x < max_x; cur_x++)
      this->buffer[cur_x + cur_y * this->width] = color;
}
