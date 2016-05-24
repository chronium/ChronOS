#include <stdio.h>

#include <kernel/panic.h>

void panic () {
  asm volatile ("cli");

  printf("Kernel panic!\n");

  while (1) {}
}
