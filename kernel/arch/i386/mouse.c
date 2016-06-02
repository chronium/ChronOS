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

static uint8_t mouse_cycle = 0;

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

static void mouse_handler (regs_t *ctx) {
  (void) ctx;
  uint8_t status = inb (MOUSE_STATUS);
  while (status & MOUSE_BBIT) {
    int8_t mouse_in = inb (MOUSE_PORT);

    if (status & MOUSE_F_BIT) {
      switch (mouse_cycle) {
        case 0:
          mouse_byte[0] = mouse_in;
          if (!(mouse_in & MOUSE_V_BIT)) goto read_next;
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
          mouse_cycle = 0;
          break;
      }
      goto read_next;
    }
  read_next:
    status = inb (MOUSE_STATUS);
  }
}

void init_mouse () {
  uint8_t status;
  (void) status;
  (void) mouse_handler;

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

  request_irq (MOUSE_IRQ, mouse_handler);

  uint8_t tmp = inb (0x61);
  outb (0x61, tmp | 0x80);
  outb (0x61, tmp & 0x7F);
  inb (MOUSE_PORT);
}