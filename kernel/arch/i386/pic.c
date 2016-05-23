#include <stdint.h>

#include <kernel/portio.h>
#include <kernel/pic.h>

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0
#define PIC_COMMAND 0x00
#define PIC_DATA 0x01
#define PIC_CMD_ENDINTR 0x20
#define PIC_ICW1_ICW4 0x01
#define PIC_ICW1_SINGLE 0x02
#define PIC_ICW1_INTERVAL4 0x04
#define PIC_ICW1_LEVEL 0x08
#define PIC_CMD_INIT 0x10
#define PIC_MODE_8086 0x01
#define PIC_MODE_AUDO 0x02
#define PIC_MODE_BUF_SLAVE 0x08
#define PIC_MODE_BUF_MASTRE 0x0C
#define PIC_MODE_SFNM 0x10
#define PIC_READ_IRR 0x0A
#define PIC_READ_ISR 0x0B

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

void pic_eoi_master () {
  outb (PIC_MASTER, PIC_CMD_ENDINTR);
}

void pic_eoi_slave () {
  outb (PIC_SLAVE, PIC_CMD_ENDINTR);
}

void pic_initialize () {
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
