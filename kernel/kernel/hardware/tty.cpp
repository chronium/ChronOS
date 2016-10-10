#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#include <arch/i386/portio.h>
#include <kernel/video.h>
#include <kernel/tty.h>

#include <kernel/devmanager.h>

namespace Driver {

Terminal::Terminal (const char *name):
	Video (name, DeviceType::CharDevice, 80, 25, 16),
	fgColor (0x07),
	bgColor (0x00),
	x (0),
	y (0) {
	this->Clear ();
}

Terminal::~Terminal () { }

void Terminal::PutEntry (char c, size_t x, size_t y) {
	((uint16_t *) this->GetAddress ())[x + y * this->GetWidth ()] = (uint16_t) c | (uint16_t) this->GetColor () << 8;
}

void Terminal::PutEntry (char c) {
	this->PutEntry (c, this->x, this->y);
}

void Terminal::Scroll () {
	memcpy (this->GetAddress (), (void *) (this->GetAddress () + this->GetWidth () * 2), this->GetWidth () * (this->GetHeight () - 1) * 2);
}

void Terminal::Newline () {
	this->x = 0;
	if (this->y + 1 == this->GetHeight ())
		this->Scroll ();
	else
		this->y++;
}

void Terminal::SetCursor () {
	uint16_t pos = this->x + this->y * this->GetWidth ();

	outb (0x3D4, 0x0F);
	outb (0x3D5, (uint8_t) (pos & 0xFF));

	outb (0x3D4, 0x0E);
	outb (0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void Terminal::PutC (char c) {
	switch (c) {
		case '\n':
			this->Newline ();
			break;
		case '\r':
			this->x = 0;
			break;
		case '\b':
			this->PutEntry (' ', --this->x, this->y);
			break;
		default:
			if (this->x == this->GetWidth ())
				this->Newline ();
			this->PutEntry (c);
			this->x++;
	}
	this->SetCursor ();
}

void Terminal::Write (const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		this->PutC (data[i]);
}

void Terminal::WriteS (const char *data) {
	this->Write (data, strlen (data));
}

void Terminal::Clear () {
	for (size_t y = 0; y < this->GetHeight (); y++)
		for (size_t x = 0; x < this->GetWidth (); x++)
			this->PutEntry (' ', x, y);
	this->SetCursor ();
}

size_t Terminal::Read (void *buffer, size_t len, uint32_t address) {
	(void) buffer;
	(void) len;
	(void) address;

	return 0;
}

size_t Terminal::Write (const void *buffer, size_t len, uint32_t address) {
	(void) address;

	this->Write ((const char *) buffer, len);

	return len;
}

Terminal *terminal = new Terminal("tty0");

}
