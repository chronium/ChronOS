#include <kernel/context>
#include <kernel/video.h>
#include <arch/i386/serial.h>

Context::Context (struct video *vga) {
  this->width = (uint16_t) vga->width;
  this->height = (uint16_t) vga->height;

  this->buffer = vga->buffer;
  this->vga = vga;

  this->clip_rects = new List<Rect> ();
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

void Context::draw_rect (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  this->hline (x, y, width, color);
  this->vline (x, y + 1, height - 2, color);
  this->hline (x, y + height - 1, width, color);
  this->vline (x + width - 1, y + 1, height - 2, color);
}

void Context::hline (uint16_t x, uint16_t y, uint16_t length, uint32_t color) {
  this->fill_rect (x, y, length, 1, color);
}

void Context::vline (uint16_t x, uint16_t y, uint16_t height, uint32_t color) {
  this->fill_rect (x, y, 1, height, color);
}

void Context::add_clip_rect (Rect *rect) {
  size_t i;
  Rect *current;
  List<Rect> *split_rects;

  for (i = 0; i < this->clip_rects->getSize (); ) {
    current = this->clip_rects->get (i);

    if (!(current->getLeft   () <= rect->getRight  () &&
          current->getRight  () >= rect->getLeft   () &&
          current->getTop    () <= rect->getBottom () &&
          current->getBottom () >= rect->getTop    ())) {
      i++;
      continue;
    }

    this->clip_rects->remove (i);
    split_rects = current->split (rect);
    delete current;

    while (split_rects->getSize ()) {
      this->clip_rects->insert (split_rects->remove (0));
    }

    i = 0;
  }

  this->clip_rects->insert (rect);
}

void Context::clear_clip_rects () {
  Rect *current;

  while (this->clip_rects->getSize ()) {
    current = (Rect *) this->clip_rects->remove (0);
    delete current;
  }
}
