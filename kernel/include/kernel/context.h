#ifndef _KERNEL_CONTEXT_H
#define _KERNEL_CONTEXT_H

#include <kernel/video.h>
#include <kernel/list.h>
#include <kernel/rect.h>

class Context {
public:
  Context (uint16_t width, uint16_t height);

  void fill_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
  void clip_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, Rect *clip_area, uint32_t color);
  void draw_rect (int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);

  void draw_bitmap (int16_t x, int16_t y, uint16_t width, uint16_t height, const uint16_t *bitmap, uint32_t color);

  inline void set_pixel (int16_t x, int16_t y, uint32_t color) __attribute__((always_inline)) {
    if(x >= 0 && x < this->GetWidth() && y >= 0 && y < this->GetHeight())
      this->buffer[x + y * this->GetWidth()] = color;
  }

  void hline (int16_t x, int16_t y, uint16_t length, uint32_t color);
  void vline (int16_t x, int16_t y, uint16_t height, uint32_t color);

  void add_clip_rect (Rect *rect);
  void subtract_clip_rect (Rect *rect);
  void intersect_clip_rect (Rect *rect);
  void clear_clip_rects ();

  inline void Clear () {
    for (int y = 0; y < this->height; y++)
      for (int x = 0; x < this->width; x++)
        this->buffer[x + y * this->width] = 0;

    this->SwapBuffers ();
  }

  inline uint16_t GetWidth () const { return this->width; };
  inline uint16_t GetHeight () const { return this->height; };

  inline uint32_t *GetBuffer () const { return this->buffer; };

  inline void SetXTrans (int16_t val) { this->translate_x = val; };
  inline void SetYTrans (int16_t val) { this->translate_y = val; };

  inline List<Rect> *getClipRects () const { return this->clip_rects; };

  inline virtual void SwapBuffers () { }
private:
  uint32_t *buffer;

  uint16_t width;
  uint16_t height;

  int16_t translate_x;
  int16_t translate_y;

  List<Rect> *clip_rects;
};

#endif
