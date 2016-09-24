#include <kernel/context.h>
#include <kernel/video.h>
#include <arch/i386/serial.h>

Context::Context (uint32_t *buffer, uint16_t width, uint16_t height) :
  buffer (buffer), width (width), height (height) {
  this->clip_rects = new List<Rect> ();
}

void Context::clip_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, Rect *clip_area, uint32_t color) {
  int cur_x;
  int max_x = x + width;
  int max_y = y + height;

  if (x < clip_area->getLeft ())
    x = clip_area->getLeft ();

  if (y < clip_area->getTop ())
    y = clip_area->getTop ();

  if (max_x > clip_area->getRight () + 1)
    max_x = clip_area->getRight () + 1;

  if (max_y > clip_area->getBottom () + 1)
    max_y = clip_area->getBottom () + 1;

  for (; y < max_y; y++)
    for (cur_x = x; cur_x < max_x; cur_x++)
      this->set_pixel(cur_x, y, color);
}

void Context::fill_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color) {
  size_t start_x, cur_x, cur_y, end_x, end_y;
  size_t max_x = x + width;
  size_t max_y = y + height;
  size_t i;

  (void) start_x;
  (void) cur_x;
  (void) cur_y;
  (void) end_x;
  (void) end_y;
  (void) max_x;
  (void) max_y;

  Rect *clip_area;

  if (this->clip_rects->getSize ()) {
    for (i = 0; i < this->clip_rects->getSize (); i++) {
      clip_area = this->clip_rects->get (i);
      this->clip_rect (x, y, width, height, clip_area, color);
    }
  } else {
    Rect screen_area (0, 0, this->height - 1, this->width - 1);

    this->clip_rect (x, y, width, height, &screen_area, color);
  }
}

void Context::draw_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color) {
  this->hline (x, y, width, color);
  this->vline (x, y + 1, height - 2, color);
  this->hline (x, y + height - 1, width, color);
  this->vline (x + width - 1, y + 1, height - 2, color);
}

void Context::draw_bitmap (int16_t x, int16_t y, uint16_t width, uint16_t height, const uint16_t *bitmap, uint32_t color) {
  for (int16_t cur_y = 0; cur_y < height; cur_y++) {
    for (int16_t cur_x = 0; cur_x < width; cur_x++) {
      if (*(bitmap + cur_y) & (1 << cur_x))
        this->set_pixel(x + cur_x, y + cur_y, color);
    }
  }
}

void Context::hline (int16_t x, int16_t y, uint16_t length, uint32_t color) {
  this->fill_rect (x, y, length, 1, color);
}

void Context::vline (int16_t x, int16_t y, uint16_t height, uint32_t color) {
  this->fill_rect (x, y, 1, height, color);
}

void Context::add_clip_rect (Rect *rect) {
  this->subtract_clip_rect (rect);

  this->clip_rects->insert (rect);
}

void Context::subtract_clip_rect (Rect *rect) {
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

    delete split_rects;

    i = 0;
  }
}

void Context::clear_clip_rects () {
  Rect *current;

  while (this->clip_rects->getSize ()) {
    current = this->clip_rects->remove (0);
    delete current;
  }
}
