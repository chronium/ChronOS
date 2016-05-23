#ifndef _KERNEL_IOPORT_H
#define _KERNEL_IOPORT_H

#if !(defined (__i386__) || defined (__x86_64__))
#error "This hardware platform does not have IO ports"
#endif

#include <stdint.h>

__attribute__ ((unused))
static inline uint8_t outb (uint16_t port, uint8_t val) {
  asm volatile ("outb %1, %0" : : "dN" (port), "a" (val));
  return val;
}

__attribute__ ((unused))
static inline uint16_t outw (uint16_t port, uint16_t val) {
  asm volatile ("outw %1, %0" : : "dN" (port), "a" (val));
  return val;
}

__attribute__ ((unused))
static inline uint32_t outl (uint16_t port, uint32_t val) {
  asm volatile ("outl %1, %0" : : "dN" (port), "a" (val));
  return val;
}

__attribute__ ((unused))
static inline uint16_t inb (uint16_t port) {
  uint16_t res;
  asm volatile ("inb %1, %0" : "=a" (res) : "dN" (port));
  return res;
}

__attribute__ ((unused))
static inline uint8_t inw (uint16_t port) {
  uint16_t res;
  asm volatile ("inbw %1, %0" : "=a" (res) : "dN" (port));
  return res;
}

__attribute__ ((unused))
static inline uint8_t inl (uint16_t port) {
  uint32_t res;
  asm volatile ("inl %1, %0" : "=a" (res) : "dN" (port));
  return res;
}
#endif
