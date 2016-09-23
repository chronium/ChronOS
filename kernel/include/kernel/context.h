#ifndef _KERNEL_CONTEXT_H
#define _KERNEL_CONTEXT_H

#include <kernel/video.h>
#include <kernel/list.h>
#include <kernel/rect.h>

class Context {
public:
  Context (struct video *vga);

  void fill_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
  void clip_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, Rect *clip_area, uint32_t color);
  void draw_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);

  void draw_bitmap (int16_t x, int16_t y, uint16_t width, uint16_t height, const uint16_t *bitmap, uint32_t color);

  void set_pixel (int16_t x, int16_t y, uint32_t color);

  void hline (int16_t x, int16_t y, uint16_t length, uint32_t color);
  void vline (int16_t x, int16_t y, uint16_t height, uint32_t color);

  void add_clip_rect (Rect *rect);
  void subtract_clip_rect (Rect *rect);
  void clear_clip_rects ();

  inline uint16_t GetWidth () const { return this->width; };
  inline uint16_t GetHeight () const { return this->height; };

  inline List<Rect> *getClipRects () const { return this->clip_rects; };

  inline struct video *getVGA () const { return this->vga; };
private:
  uint32_t *buffer;

  struct video *vga;

  uint16_t width;
  uint16_t height;

  List<Rect> *clip_rects;
};

#endif
