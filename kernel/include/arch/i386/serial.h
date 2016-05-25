#ifndef _ARCH_I386_SERIAL_H
#define _ARCH_I386_SERIAL_H

#include <stdint.h>

#include <kernel/device.h>

extern struct device *COM1;
extern struct device *COM2;
extern struct device *COM3;
extern struct device *COM4;

#define COM1_PORT  0x3F8
#define COM2_PORT  0x2F8
#define COM3_PORT  0x3E8
#define COM4_PORT  0x2E8

struct serial_dev {
  uint16_t com_port;
};

struct device *create_serial (uint16_t, const char *);
void serial_enable_port (uint16_t);

void serial_initialize ();

#endif
