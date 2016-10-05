#include <stdio.h>
#include <stdint.h>

#include <kernel/panic.h>

void panic () {
  asm volatile ("cli");

  printf("Kernel panic!\n");

  while (1) {}
}

void assert_failed (const char *file, uint32_t line, const char *desc) {
  asm volatile ("cli");
  printf ("Kernel assertion failed: %s\n", desc);
  printf ("\t%s:%d\n", file, line);
  printf ("System halted!\n");

  while (1) {
    asm volatile ("cli");
    asm volatile ("hlt");
  }
}
