#ifndef _ARCH_I386_MOUSE_H
#define _ARCH_I386_MOUSE_H

#include <stddef.h>
#include <stdint.h>

extern int8_t mouse_dx;
extern int8_t mouse_dy;

void init_mouse ();

#endif
