#ifndef _ARCH_I386_MOUSE_H
#define _ARCH_I386_MOUSE_H

#include <stddef.h>
#include <stdint.h>

int8_t mouse_dx;
int8_t mouse_dy;
uint8_t mouse_byte[4];

void init_mouse ();

#endif
