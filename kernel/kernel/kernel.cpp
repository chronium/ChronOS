#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/libc.h>
#include <kernel/icxxabi.h>

#include <sys/stat.h>

#include <fcntl.h>
#include <dirent.h>
#include <initializer_list.h>

#include <initializer_list.h>

#include <kernel/assert.h>

#include <kernel/tty.h>
#include <kernel/heap.h>
#include <kernel/video.h>
#include <kernel/context.h>
#include <kernel/vga_context.h>
#include <kernel/window.h>
#include <kernel/list.h>
#include <kernel/desktop.h>
#include <kernel/ramdisk.h>
#include <kernel/fs/tar.h>
#include <kernel/fs/vfs.h>
#include <kernel/fs/initrd.h>

#include <kernel/devmanager.h>

#define _GRAPHICS 0

#include <arch/i386/idt.h>
#include <arch/i386/gdt.h>
#include <arch/i386/paging.h>
#include <arch/i386/serial.h>
#include <arch/i386/mouse.h>
#include <arch/i386/keyboard.h>

#include <arch/i386/multiboot.h>

#include <arch/i386/keyboard.h>

multiboot_info_t *mboot_info;
extern "C"
void kearly (multiboot_info_t *_mboot_info) {
	mboot_info = _mboot_info;
	init_kheap (*(uint32_t *)(mboot_info->mods_addr + 4));
}

typedef uint32_t (*test) ();

extern "C"
void kmain (void) {
	init_gdt ();
	puts ("GDT initialized");
	init_idt ();
	puts ("IDT initialized");
	init_paging ();
	puts ("Paging initialized");

	const char *serial_test = "Hello Serial World!\n\r";
	Driver::COM1.Write (serial_test, strlen (serial_test), 0);
	puts ("Serial initialized");

	init_keyboard ();
	printf ("Keyboard initialized\n");
	init_mouse ();
	printf ("Mouse initialized\n");
	asm volatile ("sti");

	puts ("\nWelcome to ChronOS, well, the kernel to be more specific.");

	uint32_t tst = ((test) *(uint32_t *)(mboot_info->mods_addr)) ();
	printf ("%x\n", tst);
	assert (tst == 0xCAFEBABE);

	__cxa_finalize (0);

	for (;;) asm ("hlt");
}
