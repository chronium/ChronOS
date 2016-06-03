#include <stdint.h>

#include <arch/i386/portio.h>
#include <arch/i386/pic.h>

uint16_t pic_read_irr () {
  outb (PIC_MASTER + PIC_COMMAND, PIC_READ_IRR);
  outb (PIC_SLAVE + PIC_COMMAND, PIC_READ_IRR);

  return inb (PIC_MASTER + PIC_COMMAND) << 0 |
         inb (PIC_SLAVE  + PIC_COMMAND) << 8;
}

uint16_t pic_read_isr () {
  outb (PIC_MASTER + PIC_COMMAND, PIC_READ_ISR);
  outb (PIC_SLAVE + PIC_COMMAND, PIC_READ_ISR);

  return inb (PIC_MASTER + PIC_COMMAND) << 0 |
         inb (PIC_SLAVE  + PIC_COMMAND) << 8;
}

void pic_remap () {
  uint8_t map_irqs_at = 32;
  uint8_t master_mask = 0;
  uint8_t slave_mask = 0;

  outb (PIC_MASTER + PIC_COMMAND, PIC_CMD_INIT | PIC_ICW1_ICW4);
  outb (PIC_SLAVE + PIC_COMMAND, PIC_CMD_INIT | PIC_ICW1_ICW4);

  outb (PIC_MASTER + PIC_DATA, map_irqs_at + 0);
  outb (PIC_SLAVE + PIC_DATA, map_irqs_at + 8);

  outb (PIC_MASTER + PIC_DATA, 0x04);
  outb (PIC_SLAVE + PIC_DATA, 0x02);

  outb (PIC_MASTER + PIC_DATA, PIC_MODE_8086);
  outb (PIC_SLAVE + PIC_DATA, PIC_MODE_8086);

  outb (PIC_MASTER + PIC_DATA, master_mask);
  outb (PIC_SLAVE + PIC_DATA, slave_mask);
}
