#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/device.h>
#include <arch/i386/serial.h>
#include <arch/i386/portio.h>

namespace Driver {

Serial::Serial (int id, const char *name, uint16_t port) :
  Device (id, name, DeviceType::CharDevice),
  port (port) {
  this->EnablePort ();
}

Serial::~Serial () { }

void Serial::EnablePort () {
  outb (this->port + 1, 0x00);
  outb (this->port + 3, 0x80);
  outb (this->port + 0, 0x03);
  outb (this->port + 1, 0x00);
  outb (this->port + 3, 0x03);
  outb (this->port + 2, 0xC7);
  outb (this->port + 4, 0x0B);
}

size_t Serial::Read (void *buffer, size_t len, uint32_t address) {
  (void) buffer;
  (void) len;
  (void) address;

  return 0;
}

size_t Serial::Write (const void *buffer, size_t len, uint32_t address) {
  const char *data = (const char *) buffer;

  for (size_t i = 0; i < len; i++)
    this->PutC (data[address + i]);

  return len;
}

void Serial::PutC (char c) {
  while (!this->Empty ()) asm volatile ("hlt");
  outb (this->port, c);
}

bool Serial::Empty () {
  return inb (this->port + 5) & 0x20;
}

}
