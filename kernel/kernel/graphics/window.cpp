#include <kernel/window.h>
#include <kernel/list.h>
#include <arch/i386/serial.h>
#include <stdbool.h>

Window::Window (Context *context, int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t flags)
  : x (x), y (y), width (width), height (height), context (context), parent (nullptr), flags (flags), last_buttons_state (0), drag_child (nullptr), drag_off_x (0), drag_off_y (0) {
    this->children = new List<Window> ();
}

void Window::OnDraw () {
  this->context->fill_rect (0, 0, this->width, this->height, WIN_BGCOLOR);
}

Window *Window::CreateWindow (int16_t x, int16_t y, uint16_t width, uint16_t height) {
  Window *window = new Window (this->context, x, y, width, height);
  window->parent = this;

  this->children->add (window);

  return window;
}

List<Window> *Window::getWindowsAbove (Window *window) {
  int i;
  Window *current;
  List<Window> *return_list = new List<Window> ();

  for (i = 0; i < this->children->Count (); i++)
    if (window == this->children->get (i))
      break;

  for (; i < this->children->Count (); i++) {
    current = this->children->get (i);

    if (current->GetX () <= (window->GetX () + window->GetWidth() - 1) &&
       (current->GetX () + current->GetWidth () - 1) >= window->GetX () &&
       current->GetY () <= (window->GetY () + window->GetHeight () - 1) &&
       (window->GetY () + window->GetHeight() - 1) >= window->GetY ())
        return_list->add (current);
  }

  return return_list;
}

void Window::apply_bound_clip (bool recursion) {
  Rect *temp;
  int16_t screen_x, screen_y;
  List<Window> *clip_windows;
  Window *clip_window;

  screen_x = this->GetScreenX ();
  screen_y = this->GetScreenY ();

  if ((!(this->flags & WIN_NODECORATION)) && recursion) {
    screen_x += WIN_BORDERWIDTH;
    screen_y += WIN_TITLEHEIGHT;
    temp = new Rect (screen_y, screen_x,
                     screen_y + this->height - WIN_TITLEHEIGHT - WIN_BORDERWIDTH - 1,
                     screen_x + this->width - (2 * WIN_BORDERWIDTH) - 1);
  } else {
    temp = new Rect (screen_y, screen_x,
                     screen_y + this->height - 1,
                     screen_x + this->width - 1);
  }

  if (!this->parent) {
    this->GetContext ()->add_clip_rect (temp);
    return;
  }

  this->parent->apply_bound_clip (true);

  this->GetContext ()->intersect_clip_rect (temp);

  clip_windows = this->getWindowsAbove (this);
  while (clip_windows->Count ()) {
    clip_window = clip_windows->remove (0);

    if (clip_window == this)
      continue;

    screen_x = clip_window->GetScreenX ();
    screen_y = clip_window->GetScreenY ();

    temp = new Rect (screen_y, screen_x,
                     screen_y + clip_window->GetHeight () - 1,
                     screen_x + clip_window->GetWidth () - 1);
    this->GetContext ()->subtract_clip_rect (temp);
    delete temp;
  }

  delete clip_windows;
}

void Window::draw_border () {
  int i;
  int screen_x = this->GetScreenX ();
  int screen_y = this->GetScreenY ();

  this->GetContext ()->draw_rect (screen_x, screen_y, this->GetWidth (), this->GetHeight (), WIN_BORDERCOLOR);

  for (i = 0; i < WIN_BORDERWIDTH; i++)
    this->GetContext ()->hline (screen_x + WIN_BORDERWIDTH,
                                screen_y + i + WIN_TITLEHEIGHT - WIN_BORDERWIDTH,
                                this->GetWidth () - (2 * WIN_BORDERWIDTH),
                                WIN_BORDERCOLOR);

  this->GetContext ()->fill_rect (screen_x + WIN_BORDERWIDTH,
                                  screen_y + WIN_BORDERWIDTH,
                                  this->GetWidth () - 2,
                                  10,
                                  WIN_TITLECOLOR);
}

void Window::paint () {
  int i;
  int16_t screen_x, screen_y, child_screen_x, child_screen_y;
  Window *current;
  Rect *temp;

  this->apply_bound_clip (false);

  screen_x = this->GetScreenX ();
  screen_y = this->GetScreenY ();

  if (!(this->flags & WIN_NODECORATION)) {
    this->draw_border ();

    screen_x += WIN_BORDERWIDTH;
    screen_y += WIN_TITLEHEIGHT;

    temp = new Rect (screen_y, screen_x,
                     screen_y + this->height - WIN_TITLEHEIGHT - WIN_BORDERWIDTH - 1,
                     screen_x + this->width - (2 * WIN_BORDERWIDTH) - 1);
    this->GetContext ()->intersect_clip_rect (temp);
  }

  for (i = 0; i < this->children->Count (); i++) {
    current = this->children->get (i);

    child_screen_x = current->GetScreenX ();
    child_screen_y = current->GetScreenY ();

    temp = new Rect (child_screen_y, child_screen_x,
                     child_screen_y + current->GetHeight () - 1,
                     child_screen_x + current->GetWidth () - 1);
    this->GetContext ()->subtract_clip_rect (temp);
    delete temp;
  }

  this->GetContext ()->SetXTrans (screen_x);
  this->GetContext ()->SetYTrans (screen_y);
  this->OnDraw ();

  this->GetContext ()->clear_clip_rects ();
  this->GetContext ()->SetXTrans (0);
  this->GetContext ()->SetYTrans (0);

  for (i = 0; i < this->children->Count (); i++) {
    current = this->children->get (i);
    current->paint ();
  }
}


void Window::process_mouse (int16_t x, int16_t y, uint8_t mouse_buttons) {
  int i;
  Window *child;

  for (i = this->children->Count () - 1; i >= 0; i--) {
    child = this->children->get (i);

    if (!(x >= child->GetX () && x < (child->GetX () + child->GetWidth ()) &&
          y >= child->GetY () && y < (child->GetY () + child->GetHeight ())))
      continue;


    if (mouse_buttons && !this->last_buttons_state) {
      this->children->remove (i);
      this->children->add (child);

      if (!(child->flags && WIN_NODECORATION) &&
            y >= child->GetY () && y < (child->GetY () + WIN_TITLEHEIGHT)) {
        this->drag_off_x = x - child->GetX ();
        this->drag_off_y = y - child->GetY ();
        this->drag_child = child;

        break;
      }
    }

    child->process_mouse (x - child->GetX (), y - child->GetY (), mouse_buttons);
    break;
  }

  if (!mouse_buttons)
    this->drag_child = nullptr;

  if (this->drag_child) {
    this->drag_child->setX (x - this->drag_off_x);
    this->drag_child->setY (y - this->drag_off_y);
  }

  if (mouse_buttons && !this->last_buttons_state)
    this->OnMouseDown (x, y);

  this->last_buttons_state = mouse_buttons;
}
