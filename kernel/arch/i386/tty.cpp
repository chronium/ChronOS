#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <arch/i386/vga.h>
#include <arch/i386/portio.h>
#include <kernel/video.h>
#include <kernel/tty.h>

#define MAKE_COLOR(FG, BG) FG | BG << 4

static textmode_t *textmode;

void term_putentry (textmode_t *tty, char, uint8_t, size_t, size_t);

struct textmode *term_init (struct video *video) {
	textmode = (textmode_t *) malloc (sizeof (textmode_t));

	textmode->address 	= (uint16_t *) 0xB8000;
	textmode->fg_color = 0x07;
	textmode->bg_color = 0x00;

	textmode->video = video;

	for (size_t y = 0; y < video->height; y++)
		for (size_t x = 0; x < video->width; x++)
			term_putentry (textmode, ' ', MAKE_COLOR (textmode->fg_color, textmode->bg_color), x, y);

	return textmode;
}

void term_putentry (textmode_t *tty, char c, uint8_t color, size_t x, size_t y) {
	tty->address[x + y * tty->video->width] = make_vgaentry (c, color);
}

static void term_scroll (textmode_t *tty) {
	memcpy (tty->address, tty->address + (tty->video->width), (tty->video->height - 1) * tty->video->width);
	for (size_t x = 0; x < tty->video->width; x++)
		term_putentry (tty, ' ', MAKE_COLOR (tty->fg_color, tty->bg_color), x, VGA_HEIGHT - 1);
}

static void term_newline (textmode_t *tty) {
	tty->x = 0;
	if (tty->y + 1 == tty->video->height)
		term_scroll (tty);
	else
		tty->y++;
}

void term_putc (textmode_t *tty, char c) {
	if (c == '\n')
		term_newline (tty);
	else if (c == '\r')
		tty->x = 0;
	else {
		if (tty->x == tty->video->width)
			term_newline (tty);
		term_putentry (tty, c, MAKE_COLOR (tty->fg_color, tty->bg_color), tty->x, tty->y);
		tty->x++;
		term_set_cursor (tty, tty->x, tty->y);
	}
}

void term_write (textmode_t *tty, const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		term_putc (tty, data[i]);
}

void term_writes (textmode_t *tty, const char *data) {
	term_write (tty, data, strlen (data));
}

size_t term_write_dev (void *tag, const void *data, size_t len, uint32_t addr) {
	term_write ((textmode_t *) tag, (const char *) data, len);

	(void) addr;

	return len;
}

void term_set_cursor (textmode_t *tty, size_t x, size_t y) {
	uint16_t pos = x + y * tty->video->width;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));

	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

textmode_t *get_textmode () {
	return textmode;
}
