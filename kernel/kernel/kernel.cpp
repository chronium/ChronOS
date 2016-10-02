#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/icxxabi.h>

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

	auto *ramdisk = new Driver::Ramdisk (1, "initrd", (void *) *(uint32_t *)(mboot_info->mods_addr));

	auto *tar = FileSystem::Tar::Parse (ramdisk);

	for (int i = 0; i < tar->GetHeaders ()->Count (); i++)
		if (FileSystem::Tar::GetType (tar->GetHeaders ()->get (i)) == FileSystem::Tar::FileType::DirType)
			printf ("File %d: %s\n", i, tar->GetHeaders ()->get (i)->header->filename);

	FileSystem::tar_file_t *greet_file = tar->GetHeaders ()->get (2);
	size_t greet_file_size = FileSystem::Tar::GetSize (greet_file);
	char *greet = new char[greet_file_size + 1];
	greet[greet_file_size] = '\0';
	memcpy (greet, greet_file->content, greet_file_size);
	printf ("greet.txt: %s\n", greet);

#if _GRAPHICS == 1
	Desktop *desktop = new Desktop (new VGAContext (3, "vga"));
	desktop->CreateWindow (10, 10, 80, 50);
	desktop->CreateWindow (100, 50, 50, 60);
	desktop->CreateWindow (200, 100, 50, 50);

	while (true) {
		desktop->update_mouse (mouse_x, mouse_y, mouse_left);
	}

#else

	while (true) {
		getc ();
	}

#endif

	__cxa_finalize (0);

	for (;;) asm ("hlt");
}
