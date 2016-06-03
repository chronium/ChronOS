#ifndef _ARCH_I386_SERIAL_H
#define _ARCH_I386_SERIAL_H

#include <stdint.h>

#include <kernel/device.h>

#define COM1_PORT  0x3F8
#define COM2_PORT  0x2F8
#define COM3_PORT  0x3E8
#define COM4_PORT  0x2E8

class Serial : public Device {
public:
  Serial (int id, const char *name, uint16_t port);
  ~Serial ();

  size_t Read (void *buffer, size_t len, uint32_t address);
  size_t Write (const void *buffer, size_t len, uint32_t address);

  void EnablePort ();
  bool Empty ();
  void PutC (char c);
private:
  uint16_t port;
};

extern struct device *COM2;
extern struct device *COM3;
extern struct device *COM4;


struct serial_dev {
  uint16_t com_port;
};

struct device *create_serial (uint16_t, const char *);
void serial_enable_port (uint16_t);

void init_serial ();

#endif
