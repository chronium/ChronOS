#include <stddef.h>
#include <stdio.h>

#include <arch/i386/pic.h>
#include <arch/i386/portio.h>

inthandler_t irq_handlers[16] = { 0 };

int request_irq (int inum, inthandler_t handler) {
  if (inum > 16 || irq_handlers[inum])
    return -1;
  else
    irq_handlers[inum] = handler;
  return 0;
}

int free_irq (int inum) {
  irq_handlers[inum] = NULL;
  return 0;
}

void handle_irq (regs_t *registers) {
  int i = (registers->int_no & 0xFF) - 32;
  if (i >= 8)
    pic_eoi_slave ();
  if (irq_handlers[i])
    irq_handlers[i] (registers);
  pic_eoi_master ();
}
