#include <stddef.h>

#include <arch/i386/pic.h>
#include <arch/i386/portio.h>
#include <arch/i386/exception.h>

inthandler_t isr_handlers[256] = { 0 };

int request_isr (int inum, inthandler_t handler) {
  if (isr_handlers[inum])
    return -1;
  else
    isr_handlers[inum] = handler;
  return 0;
}

int free_isr (int inum) {
  isr_handlers[inum] = NULL;
  return 0;
}

void handle_isr (regs_t *registers) {
  int i = registers->int_no & 0xFF;
  if (isr_handlers[i])
    isr_handlers[i] (registers);
  else
    handle_exception (registers);
  pic_eoi_master ();
}
