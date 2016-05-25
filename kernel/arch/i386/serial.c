#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/device.h>
#include <arch/i386/serial.h>
#include <arch/i386/portio.h>

#define COM_PORT  0x3F8

struct device *COM1;
struct device *COM2;
struct device *COM3;
struct device *COM4;

static int serial_empty (uint16_t);
static void serial_putc (uint16_t, char);
static size_t serial_write (void *, const void *, size_t, uint32_t);

void serial_initialize () {
  COM1 = create_serial (COM1_PORT, "COM1");
  COM2 = create_serial (COM2_PORT, "COM2");
  COM3 = create_serial (COM3_PORT, "COM3");
  COM4 = create_serial (COM4_PORT, "COM4");
}

struct device *create_serial (uint16_t com_port, const char *name) {
  struct device *dev = dev_create (CHAR_DEV, name);
  dev->dev_tag = (struct serial_dev *) malloc (sizeof (struct serial_dev));
  dev->write = serial_write;

  struct serial_dev *tag = (struct serial_dev *) dev->dev_tag;
  tag->com_port = com_port;

  serial_enable_port (com_port);

  return dev;
}

void serial_enable_port (uint16_t com_port) {
  outb (com_port + 1, 0x00);
  outb (com_port + 3, 0x80);
  outb (com_port + 0, 0x03);
  outb (com_port + 1, 0x00);
  outb (com_port + 3, 0x03);
  outb (com_port + 2, 0xC7);
  outb (com_port + 4, 0x0B);
}

static size_t serial_write (void *tag, const void *data, size_t s, uint32_t addr) {
  uint16_t port = ((struct serial_dev *) tag)->com_port;

  const char *dat = (const char *) data;

  for (size_t i = 0; i < s; i++)
    serial_putc (port, dat[addr + i]);
  return s;
}

static void serial_putc (uint16_t port, char c) {
  while (!serial_empty (port));
  outb (port, c);
}

static int serial_empty (uint16_t port) {
  return inb (port + 5) & 0x20;
}
