#ifndef _ARCH_I386_MOUSE_H
#define _ARCH_I386_MOUSE_H

#include <stddef.h>
#include <stdint.h>

extern int8_t mouse_dx;
extern int8_t mouse_dy;

extern int16_t mouse_x;
extern int16_t mouse_y;

extern uint8_t mouse_left;

void init_mouse ();

#endif
