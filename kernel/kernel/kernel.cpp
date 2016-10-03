#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/libc.h>
#include <kernel/icxxabi.h>

#include <sys/stat.h>

#include <fcntl.h>

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

	auto *ramdisk = new Driver::Ramdisk (4, "initrd", (void *) *(uint32_t *)(mboot_info->mods_addr));
	auto *tar = FileSystem::Tar::Parse (ramdisk);

	auto *initrd = new FileSystem::Initrd (strdup ("/"), tar);

	VFS::InitVFS (initrd);

	int file = open ("/greet.txt", O_RDONLY);

	if (file == -1)
		puts ("Could not open /greet.txt");
	else
		printf ("/greet.txt open in fd: %d\n", file);

	struct stat st;

	if (stat ("/", &st) != -1) {
		if (S_ISDIR(st.st_mode))
			printf ("/ is a directory\n");
		else
			printf ("/ is not a directory???\n");
	}

	if (stat ("/greet.txt", &st) != -1) {
		if (S_ISREG(st.st_mode))
			printf ("/greet.txt is a file\n");
		else
			printf ("/greet.txt is not a file???\n");
	}


#if _GRAPHICS == 1
	Desktop *desktop = new Desktop (new VGAContext (3, "vga"));
	desktop->CreateWindow (10, 10, 80, 50);
	desktop->CreateWindow (100, 50, 50, 60);
	desktop->CreateWindow (200, 100, 50, 50);

	while (true) {
		desktop->update_mouse (mouse_x, mouse_y, mouse_left);
	}

#endif

	__cxa_finalize (0);

	for (;;) asm ("hlt");
}
