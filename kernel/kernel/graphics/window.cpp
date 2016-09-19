#include <kernel/window>
#include <kernel/list>

Window::Window (Context *context, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
  x (x), y (y), width (width), height (height), context (context) {
  this->fill_color = 0xFF000000 |            //Opacity
                   pseudo_rand_8() << 16 | //B
                   pseudo_rand_8() << 8  | //G
                   pseudo_rand_8();        //R
}

void Window::paint () {
  this->context->fill_rect (this->x, this->y, this->width, this->height, this->fill_color);
}
