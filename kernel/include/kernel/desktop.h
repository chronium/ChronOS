#ifndef _KERNEL_DESKTOP_H
#define _KERNEL_DESKTOP_H

#include <kernel/window.h>
#include <kernel/context.h>
#include <kernel/list.h>

#include <stdint.h>

class Desktop {
public:
  Desktop (Context *context): context (context) {
    this->windows = new List<Window> ();
  }
  ~Desktop () {
    delete windows;
  }

  Window *createWindow (uint16_t x, uint16_t y, uint16_t width, uint16_t height);

  void update (int16_t mouse_x, int16_t mouse_y, uint8_t mouse_buttons);
  void paint ();

private:
  List<Window> *windows;
  Window *dragged_window;

  uint16_t drag_off_x;
  uint16_t drag_off_y;

  Context *context;

  uint8_t last_buttons_state;
  uint16_t mouse_x;
  uint16_t mouse_y;
};

#endif