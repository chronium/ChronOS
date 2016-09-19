#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/icxxabi.h>

#include <kernel/tty.h>
#include <kernel/heap.h>
#include <kernel/video.h>
#include <kernel/context>
#include <kernel/window>
#include <kernel/list>
#include <kernel/desktop>

#define _GRAPHICS 1

#include <arch/i386/idt.h>
#include <arch/i386/gdt.h>
#include <arch/i386/paging.h>
#include <arch/i386/serial.h>
#include <arch/i386/mouse.h>
#include <arch/i386/keyboard.h>

#include <arch/i386/multiboot.h>

multiboot_info_t *mboot_info;
extern "C"
void kearly (multiboot_info_t *_mboot_info) {
	mboot_info = _mboot_info;
	init_kheap (*(uint32_t *)(mboot_info->mods_addr + 4));
#if _GRAPHICS == 1
	init_video (true);
#else
	init_video (false);
#endif
}

Driver::Terminal *terminal = new Driver::Terminal (0, "tty0");

extern "C"
void kmain (void) {
	init_gdt ();
	puts ("GDT initialized");
	init_idt ();
	puts ("IDT initialized");
	init_paging ();
	puts ("Paging initialized");
	//Driver::Serial COM1 (0, "COM1", COM1_PORT);
	const char *serial_test = "Hello Serial World!\n\r";
	Driver::COM1.Write (serial_test, strlen (serial_test), 0);
	puts ("Serial initialized");

	init_keyboard ();
	printf ("Keyboard initialized\n");
	init_mouse ();
	printf ("Mouse initialized\n");
	asm volatile ("sti");

	puts ("\nWelcome to ChronOS, well, the kernel to be more specific.");

#if _GRAPHICS == 1
	//screen_loop ();

	Context *context = new Context (video_inst);

	Desktop *desktop = new Desktop (context);
	desktop->createWindow (10, 10, 80, 50);
	desktop->createWindow (100, 50, 50, 60);
	desktop->createWindow (200, 100, 50, 50);

	while (true) {
		desktop->update (mouse_x, mouse_y, mouse_left);
	}

#endif

	__cxa_finalize (0);

	for (;;) asm ("hlt");
}
