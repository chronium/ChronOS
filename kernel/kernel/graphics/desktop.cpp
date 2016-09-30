#include <kernel/desktop.h>
#include <kernel/rect.h>
#include <kernel/video.h>

#include <arch/i386/serial.h>

/*static uint16_t cursor_black[] = { 0x01FF, 0x0081, 0x0041, 0x0021, 0x0011, 0x0009, 0x0005, 0x0003, 0x0001 };
static uint16_t cursor_40[] = { 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001 };
static uint16_t cursor_80[] = { 0x001C, 0x000E, 0x0007, 0x0003, 0x0001 };*/

Desktop::Desktop (Context *context):
  Window (context, 0, 0, context->GetWidth (), context->GetHeight (), WIN_NODECORATION) {
}

void Desktop::OnDraw () {
  this->GetContext ()->fill_rect (0, 0, this->GetWidth (), this->GetHeight (), 0x3399FF);
}

void Desktop::OnUpdate () { }

void Desktop::update_mouse (int16_t mouse_x, int16_t mouse_y, uint8_t mouse_buttons) {
  this->OnUpdate ();
  this->process_mouse (mouse_x, mouse_y, mouse_buttons);
  this->paint ();

  this->GetContext ()->fill_rect (mouse_x, mouse_y, 1, 1, 0);

  this->GetContext ()->swap_buff ();
}
