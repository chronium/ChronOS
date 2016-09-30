#ifndef _KERNEL_WINDOW_H
#define _KERNEL_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#include <kernel/context.h>

#define WIN_BGCOLOR     0xFFBBBBBB
#define WIN_TITLECOLOR  0xFF7092BE
#define WIN_BORDERCOLOR 0xFF000000

#define WIN_TITLEHEIGHT 12
#define WIN_BORDERWIDTH 1

#define WIN_NODECORATION 0x1

class Window {
public:
  Window (Context *context, int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t flags = 0);

  ~Window () {
    delete children;
    context = nullptr;
    parent = nullptr;
    drag_child = nullptr;
  }

  virtual void OnDraw ();
  virtual void OnUpdate () { }
  virtual void OnMouseDown (int16_t x, int16_t y) { (void) x; (void) y; }

  void paint ();
  void process_mouse (int16_t x, int16_t y, uint8_t mouse_buttons);
  void draw_border ();

  Window *CreateWindow (int16_t x, int16_t y, uint16_t width, uint16_t height);

  void apply_bound_clip (bool recursion);
  List<Window> *getWindowsAbove (Window *window);

  inline void setX (int16_t val) { this->x = val; };
  inline void setY (int16_t val) { this->y = val; };

  inline int16_t GetX () const { return this->x; };
  inline int16_t GetY () const { return this->y; };

  inline int16_t GetScreenX () const {
    if (this->parent)
      return this->x + this->parent->GetScreenX ();

    return this->x;
  };
  inline int16_t GetScreenY () const {
    if (this->parent)
      return this->y + this->parent->GetScreenY ();

    return this->y;
  };

  inline uint16_t GetWidth () const { return this->width; };
  inline uint16_t GetHeight () const { return this->height; };

  inline uint16_t GetFlags () const { return this->flags; };

  inline Context *GetContext () const { return this->context; };

private:
  int16_t x;
  int16_t y;

  uint16_t width;
  uint16_t height;

  Context *context;
  Window *parent;

  uint16_t flags;

  List<Window> *children;

  uint8_t last_buttons_state;

  Window *drag_child;
  uint16_t drag_off_x;
  uint16_t drag_off_y;
};

#endif
