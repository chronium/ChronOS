#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <arch/i386/vga.h>
#include <arch/i386/portio.h>
#include <kernel/tty.h>

size_t term_row;
size_t term_col;
uint8_t term_color;
uint16_t *term_buff;

void term_putentry (char, uint8_t, size_t, size_t);

void term_init (void) {
	term_row = term_col = 0;
	term_color = make_color (COLOR_LIGHT_GREY, COLOR_BLACK);
	term_buff = VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++)
		for (size_t x = 0; x < VGA_WIDTH; x++)
			term_putentry (' ', term_color, x, y);			
}

void term_setcolor (uint8_t color) {
	term_color = color;
}

void term_putentry (char c, uint8_t color, size_t x, size_t y) {
	term_buff[x + y * VGA_WIDTH] = make_vgaentry (c, color);
}

static void term_scroll () {
	memcpy (term_buff, term_buff + (VGA_WIDTH), (VGA_HEIGHT - 1) * VGA_WIDTH);
	for (size_t x = 0; x < VGA_WIDTH; x++)
		term_putentry (' ', term_color, x, VGA_HEIGHT - 1);
}

static void term_newline () {
	term_col = 0;
	if (term_row + 1 == VGA_HEIGHT)
		term_scroll ();
	else
		term_row++;
}

void term_putc (char c) {
	if (c == '\n')
		term_newline ();
	else if (c == '\r')
		term_col = 0;
	else {
		if (term_col == VGA_WIDTH)
			term_newline ();
		term_putentry (c, term_color, term_col, term_row);
		term_col++;
		term_set_cursor (term_col, term_row);
	}
}

void term_write (const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		term_putc (data[i]);
}

void term_writes (const char *data) {
	term_write (data, strlen (data));
}

void term_set_cursor (size_t x, size_t y) {
	uint16_t pos = x + y * VGA_WIDTH;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));

	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
