#include <stdio.h>

#include <arch/i386/portio.h>
#include <arch/i386/keyboard.h>
#include <arch/i386/pic.h>

static void keyboard_wait(void) {
	while(inb(0x64) & 2);
}

static void keyboard_handler (regs_t *ctx) {
  (void) ctx;

  keyboard_wait ();
  inb (0x60);
}

void init_keyboard () {
  request_irq (1, keyboard_handler);
}
