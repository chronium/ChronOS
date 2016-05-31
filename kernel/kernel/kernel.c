#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/heap.h>
#include <kernel/video.h>

#if defined(__i386__)
#include <arch/i386/idt.h>
#include <arch/i386/pic.h>
#include <arch/i386/paging.h>
#include <arch/i386/serial.h>

#include <arch/i386/multiboot.h>

multiboot_info_t *mboot_info;

void kearly (multiboot_info_t *_mboot_info) {
	mboot_info = _mboot_info;
	init_kheap (*(uint32_t *)(mboot_info->mods_addr + 4));
	init_video (true);
	printf("Heap initialized\n");
}
#else
void kearly (void) {
	init_video (true);
}
#endif

void kmain (void) {
#if defined(__i386__)
	init_idt ();
	puts ("IDT initialized");
	init_pic ();
	puts ("PIC initialized");
	init_paging ();
	puts ("Paging initialized");
	init_serial ();
	puts ("Serial initialized");
	dev_write (COM1, "Helo Serial World!\n", strlen ("Hello Serial World!\n"), 0);
#endif

	puts ("\nWelcome to ChronOS, well, the kernel to be more specific.");
}
