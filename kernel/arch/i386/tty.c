#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <arch/i386/vga.h>
#include <arch/i386/portio.h>
#include <kernel/tty.h>

#define MAKE_COLOR(FG, BG) FG | BG << 4

static struct textmode *tty;

void term_putentry (char, uint8_t, size_t, size_t);

struct textmode *term_init (struct video *video) {
	tty = (struct textmode *) malloc (sizeof (struct textmode));

	tty->address 	= (uint16_t *) 0xB8000;
	tty->fg_color = 0x07;
	tty->bg_color = 0x00;

	tty->video = video;

	for (size_t y = 0; y < video->height; y++)
		for (size_t x = 0; x < video->width; x++)
			term_putentry (' ', MAKE_COLOR (tty->fg_color, tty->bg_color), x, y);

	return tty;
}

void term_putentry (char c, uint8_t color, size_t x, size_t y) {
	tty->address[x + y * tty->video->width] = make_vgaentry (c, color);
}

static void term_scroll () {
	memcpy (tty->address, tty->address + (tty->video->width), (tty->video->height - 1) * tty->video->width);
	for (size_t x = 0; x < tty->video->width; x++)
		term_putentry (' ', MAKE_COLOR (tty->fg_color, tty->bg_color), x, VGA_HEIGHT - 1);
}

static void term_newline () {
	tty->x = 0;
	if (tty->y + 1 == tty->video->height)
		term_scroll ();
	else
		tty->y++;
}

void term_putc (char c) {
	if (c == '\n')
		term_newline ();
	else if (c == '\r')
		tty->x = 0;
	else {
		if (tty->x == tty->video->width)
			term_newline ();
		term_putentry (c, MAKE_COLOR (tty->fg_color, tty->bg_color), tty->x, tty->y);
		tty->x++;
		term_set_cursor (tty->x, tty->y);
	}
}

void term_write (const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		term_putc (data[i]);
}

void term_writes (const char *data) {
	term_write (data, strlen (data));
}

size_t term_write_dev (void *tag, const void *data, size_t len, uint32_t addr) {
	term_write (data, len);

	(void) tag;
	(void) addr;

	return len;
}

void term_set_cursor (size_t x, size_t y) {
	uint16_t pos = x + y * tty->video->width;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));

	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
