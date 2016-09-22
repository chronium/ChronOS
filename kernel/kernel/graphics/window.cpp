#include <kernel/window.h>
#include <kernel/list.h>

void Window::paint () {
  this->context->draw_rect (this->x, this->y, this->width, this->height, WIN_BORDERCOLOR);

  this->context->hline (this->x + 1, this->y + 11, this->width - 2, WIN_BORDERCOLOR);

  this->context->fill_rect (this->x + 1, this->y + 1, this->width - 2, 10, WIN_TITLECOLOR);
  this->context->fill_rect (this->x + 1, this->y + 12, this->width - 2, this->height - 13, WIN_BGCOLOR);
}
