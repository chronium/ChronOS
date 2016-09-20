#include <kernel/desktop.h>
#include <kernel/rect.h>
#include <kernel/video.h>

Window *Desktop::createWindow (uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  Window *window = new Window (this->context, x, y, width, height);

  this->windows->insert (window);

  return window;
}

void Desktop::paint () {
  Window *current;
  size_t i;
  Rect *temp;

  this->context->fill_rect (0, 0, this->context->GetWidth (), this->context->GetHeight (), 0);

  this->context->clear_clip_rects ();

  for (i = 0; (current = (Window *) this->windows->get (i)); i++) {
    temp = new Rect (current->getY (), current->getX (),
                     current->getY () + current->getHeight () - 1,
                     current->getX () + current->getWidth () - 1);
    this->context->add_clip_rect (temp);
  }

  for (i = 0; i < this->context->getClipRects ()->getSize (); i++) {
    temp = (Rect *) this->context->getClipRects ()->get (i);

    this->context->draw_rect (temp->getLeft (), temp->getTop (),
                              temp->getRight () - temp->getLeft () + 1,
                              temp->getBottom () - temp->getTop () + 1,
                              0x0000FF00);
  }

  this->context->fill_rect (this->mouse_x, this->mouse_y, 10, 10, 0xFFFFFFFF);

  swap_buffers (this->context->getVGA (), 0, 0, this->context->GetWidth (), this->context->GetHeight ());
}

void Desktop::update (int16_t mouse_x, int16_t mouse_y, uint8_t mouse_buttons) {
  int i;
  Window *child;

  this->mouse_x = mouse_x;
  this->mouse_y = mouse_y;

  if (mouse_buttons) {
    if (!this->last_buttons_state)
      for (i = this->windows->getSize () - 1; i >= 0; i--) {
        child = (Window *) this->windows->get (i);

        if (mouse_x >= child->getX () && mouse_x < (child->getX () + child->getWidth ()) &&
           mouse_y >= child->getY () && mouse_y < (child->getY () + child->getHeight ())) {
             this->windows->remove (i);
             this->windows->insert (child);

             this->drag_off_x = mouse_x - child->getX ();
             this->drag_off_y = mouse_y - child->getY ();
             this->dragged_window = child;

             break;
           }
      }
  } else {
    this->dragged_window = nullptr;
  }

  if (this->dragged_window) {
    this->dragged_window->setX (mouse_x - this->drag_off_x);
    this->dragged_window->setY (mouse_y - this->drag_off_y);
  }

  this->paint ();

  this->last_buttons_state = mouse_buttons;
}
