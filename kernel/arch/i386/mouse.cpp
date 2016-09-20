#include <stdint.h>
#include <stdio.h>

#include <arch/i386/mouse.h>
#include <arch/i386/portio.h>
#include <arch/i386/pic.h>

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

#define MOUSE_IRQ 12

#define MOUSE_DEFAULT 0
#define MOUSE_SCROLLWHEEL 1
#define MOUSE_BUTTONS 2

static uint8_t mouse_cycle = 0;

static int8_t mouse_mode = MOUSE_DEFAULT;

static inline void mouse_wait (uint8_t type) {
  uint32_t timeout = 100000;

  if (type == 0) {
    while (timeout--)
      if ((inb (MOUSE_STATUS) & MOUSE_BBIT) == 1)
        return;
    return;
  } else {
    while (timeout--)
      if (!(inb (MOUSE_STATUS) & MOUSE_ABIT))
        return;
    return;
  }
}

static inline void mouse_write (uint8_t write) {
  mouse_wait (1);
  outb (MOUSE_STATUS, MOUSE_WRITE);
  mouse_wait (1);
  outb (MOUSE_PORT, write);
}

static inline uint8_t mouse_read (void) {
  mouse_wait (0);
  return inb (MOUSE_PORT);
}

static uint8_t mouse_byte[4];
int8_t mouse_dx;
int8_t mouse_dy;

int16_t mouse_x;
int16_t mouse_y;

uint8_t mouse_left;

static void mouse_handler (regs_t *ctx) {
  (void) ctx;
  uint8_t status = inb (MOUSE_STATUS);
  (void) status;
  int8_t mouse_in = inb (MOUSE_PORT);
  switch (mouse_cycle) {
    case 0:
      mouse_byte[0] = mouse_in;
      if (mouse_byte[0] & 0x01)
        mouse_left = 1;
      else
        mouse_left = 0;
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1] = mouse_in;
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2] = mouse_in;
      mouse_dx = mouse_byte[1];
      mouse_dy = mouse_byte[2];

      mouse_x += mouse_dx;
      mouse_y += -mouse_dy;

      if (mouse_x < 0)
        mouse_x = 0;
      if (mouse_y < 0)
        mouse_y = 0;

      if (mouse_x >= 320)
        mouse_x = 320 - 1;
      if (mouse_y >= 200)
        mouse_y = 200 - 1;

      mouse_cycle = 0;
      break;
    case 3:
      mouse_byte[3] = mouse_in;
      mouse_cycle = 0;
      break;
  }

  /*(void) ctx;

  uint8_t status = inb (MOUSE_STATUS);
  int8_t mouse_in = inb (MOUSE_PORT);

  if (status & MOUSE_F_BIT) {
    switch (mouse_cycle) {
      case 0:
        mouse_byte[0] = mouse_in;
        if (!(mouse_in & MOUSE_V_BIT))
          goto read_next;
        mouse_cycle++;
        break;
      case 1:
        mouse_byte[1] = mouse_in;
        mouse_cycle++;
        break;
      case 2:
        mouse_byte[2] = mouse_in;
        if (mouse_mode == MOUSE_SCROLLWHEEL || mouse_mode == MOUSE_BUTTONS) {
          mouse_cycle++;
          break;
        }
        goto finish_packet;
      case 3:
        mouse_byte[3] = mouse_in;
        goto finish_packet;
    }
    goto read_next;
finish_packet:
    mouse_cycle = 0;
    if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40)
      goto read_next;
    mouse_dx = mouse_byte[1];
    mouse_dy = mouse_byte[2];
  }
read_next:
  status = inb (MOUSE_STATUS);*/
}

void init_mouse () {
  uint8_t status, result;
  (void) status;
  (void) result;
  (void) mouse_handler;
  (void) mouse_mode;

  mouse_wait (1);
  outb (0x64, 0xA8);
  mouse_wait (1);
  outb (MOUSE_STATUS, 0x20);
  mouse_wait (0);
  status = inb (MOUSE_PORT) | 2;
  mouse_wait (1);
  outb (MOUSE_STATUS, 0x60);
  mouse_wait (1);
  outb (MOUSE_PORT, status);
  mouse_write (0xF6);
  mouse_read ();
  mouse_write (0xF4);
  mouse_read ();

  /*mouse_write (0xF2);
  mouse_read ();
  result = mouse_read ();
  mouse_write (0xF3);
  mouse_read ();
  mouse_write (200);
  mouse_read ();
  mouse_write (0xF3);
  mouse_read ();
  mouse_write (100);
  mouse_read ();
  mouse_write (0xF3);
  mouse_read ();
  mouse_write (80);
  mouse_read ();
  mouse_write (0xF2);
  result = mouse_read ();
  if (result == 3) {
    mouse_mode = MOUSE_SCROLLWHEEL;
  }*/

  request_irq (MOUSE_IRQ, mouse_handler);

  uint8_t tmp = inb (0x61);
  outb (0x61, tmp | 0x80);
  outb (0x61, tmp & 0x7F);
  inb (MOUSE_PORT);
}
