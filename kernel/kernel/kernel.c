#include <stdio.h>

#include <kernel/tty.h>

#if defined(__i386__)
#include <arch/i386/idt.h>
#include <arch/i386/pic.h>
#endif

#include <multiboot.h>

multiboot_info_t *mboot_info;

void kearly (multiboot_info_t *_mboot_info) {
	mboot_info = _mboot_info;

	term_init ();
}

void kmain (void) {
	printf ("Hello World!\n");
  	idt_initialize ();
  	puts ("IDT initialized");
  	pic_initialize ();
  	puts ("PIC initialized");
  	asm volatile ("sti");
}