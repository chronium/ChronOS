#ifndef _KERNEL_DESKTOP_H
#define _KERNEL_DESKTOP_H

#include <kernel/window.h>
#include <kernel/context.h>
#include <kernel/list.h>

#include <stdint.h>

class Desktop : public Window {
public:
  Desktop (Context *context);

  Window *createWindow (int16_t x, int16_t y, uint16_t width, uint16_t height);

  void OnUpdate ();
  void update_mouse (int16_t mouse_x, int16_t mouse_y, uint8_t mouse_buttons);
  void OnDraw ();
};

#endif
