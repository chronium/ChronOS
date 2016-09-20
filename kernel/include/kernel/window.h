#ifndef _KERNEL_WINDOW_H
#define _KERNEL_WINDOW_H

#include <stdint.h>

#include <kernel/context.h>

class Window {
public:
  Window (Context *context, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

  inline uint8_t pseudo_rand_8 () {
    static uint16_t seed = 0;

    return (uint8_t) (seed = (12657 * seed + 12345) % 256);
  }

  void paint ();

  inline void setX (uint16_t val) { this->x = val; };
  inline void setY (uint16_t val) { this->y = val; };

  inline uint16_t getX () const { return this->x; };
  inline uint16_t getY () const { return this->y; };
  inline uint16_t getWidth () const { return this->width; };
  inline uint16_t getHeight () const { return this->height; };

private:
  uint16_t x;
  uint16_t y;

  uint16_t width;
  uint16_t height;

  uint32_t fill_color;

  Context *context;
};

#endif
