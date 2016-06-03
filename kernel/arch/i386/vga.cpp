#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <arch/i386/vga.h>
#include <arch/i386/portio.h>
#include <arch/i386/mouse.h>
#include <kernel/tty.h>
#include <kernel/device.h>
#include <kernel/video.h>

// static size_t write_320x200x8 (struct device *dev, const void *data, size_t len, uint32_t addr);

uint8_t g_80x25_text[] =
{
/* MISC */
	0x67,
/* SEQ */
	0x03, 0x00, 0x03, 0x00, 0x02,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
	0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00
};

/* mode 13h */
uint8_t vga_320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

void vga_write_regs(unsigned char *regs) {
	size_t i;

	/* write MISCELLANEOUS reg */
	outb(VGA_MISC_WRITE, *regs);
	regs++;
	/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
	/* unlock CRTC registers */
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
	/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
	/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
	/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
	/* lock 16-color palette and unblank display */
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}

struct video *init_textmode () {
  struct video *video = (struct video *) malloc (sizeof (struct video));
  video->width = 80;
  video->height = 25;

  video->bpe = 2;
  video->pitch = video->width * video->bpe;

  video->size = video->height * video->pitch;
  video->mode = TEXT_SCREEN;

  struct device *dev = dev_create (CHAR_DEV, "tty0");
  //dev->write = term_write_dev;

  video->dev = dev;

	vga_write_regs (g_80x25_text);
	//term_init (video);

  return video;
}

void vga_set_palette() {
	/* set the palette */
	outb(0x03c8, 0);
	for(size_t i = 0; i < 256; i++) {
		/* 8 bit palette will be:
		 rrrgggbb
		 */
		size_t red = ((i >> 5) & 7) * 63 / 7;
		size_t green = ((i >> 2) & 7) * 63 / 7;
		size_t blue = (i & 3) * 63 / 3;

		outb(0x03c9, (uint8_t) red);
		outb(0x03c9, (uint8_t) green);
		outb(0x03c9, (uint8_t) blue);
	}
}

void clear_screen (struct video *video, uint32_t c) {
	for (size_t y = 0; y < video->height; y++)
		for (size_t x = 0; x < video->width; x++)
			video->buffer[x + y * video->width] = c;
}

void swap_buffers (struct video *video, size_t minx, size_t miny, size_t maxx, size_t maxy) {
	vga_320x200x256_t *vga = (vga_320x200x256_t *) video->dev->dev_tag;

	size_t start_index = minx + miny * video->width;
	size_t i = start_index;
	size_t line_jump = video->width - (maxx-minx);

	for (size_t y = 0; y < maxy; y++) {
		for (size_t x = 0; x < maxx; x++, i++) {
			uint32_t c = vga->buffer[i];

			uint8_t red = (((c >> 16) & 0xFF)) / 32;
			uint8_t green = (((c >> 8) & 0xFF)) / 32;
			uint8_t blue = (c & 0xFF) / 64;

			uint8_t val = (red << 5) | (green << 2) | blue;

			vga->address[i] = val;
		}
		i += line_jump;
	}
}

void draw_x_line(struct video *video, size_t x, size_t y, size_t width, uint32_t color) {
	if(y >= video->height)
		return;

	size_t end_x = x + width;

	if(end_x >= video->width)
		end_x = video->width;

	size_t indx = video->width * y + x;
	for(;x != end_x; x++, indx++)
		video->buffer[indx] = color;
}

void draw_y_line(struct video *video, size_t x, size_t y, size_t height, uint32_t color) {
	if(x >= video->width)
		return;

	size_t end_y = y + height;

	if(end_y >= video->height)
		end_y = video->height;

	size_t indx = video->width * y + x;
	for(;y != end_y; y++, indx += video->width)
		video->buffer[indx] = color;
}

void set_pixel (struct video *video, size_t x, size_t y, uint32_t c) {
	video->buffer[x + y * video->width] = c;
}

static size_t mousex;
static size_t mousey;

struct video *init_mode13h () {
	struct video *video = (struct video *) malloc (sizeof (struct video));
	video->width = 320;
	video->height = 200;

	video->bpe = 4;
	video->pitch = video->width * video->bpe;

	video->size = video->height * video->pitch;
	video->mode = PIXEL_SCREEN;

	struct device *dev = dev_create (BLOCK_DEV, "vga");

	vga_320x200x256_t *vga = (vga_320x200x256_t *) malloc (sizeof (vga_320x200x256_t));
	vga->address = (uint8_t *) 0xA0000;
	vga->buffer  = (uint32_t *) malloc (video->width * video->height * 4);
	memset (vga->buffer, 0, video->width * video->height);
	vga->size = video->width * video->height;

	dev->dev_tag = vga;

	video->buffer = vga->buffer;
	video->dev = dev;

	vga_write_regs (vga_320x200x256);
	vga_set_palette ();
	clear_screen (video, 0);

	draw_x_line (video, 0, 80, 320, 0x00FF00FF);
	draw_x_line (video, 0, 120, 320, 0x00FF00FF);
	draw_y_line (video, 0, 80, 40, 0x00FF00FF);
	draw_y_line (video, 319, 80, 40, 0x00FF00FF);

	swap_buffers (video, 0, 0, 320, 200);

	mousex = video->width / 2;
	mousey = video->height / 2;

	return video;
}

void screen_loop () {
	while (true) {
		if (mouse_dx == 0 || mouse_dy == 0)
			asm ("hlt");
		mousex += mouse_dx;
		mousey -= mouse_dy;

		mouse_dx = 0;
		mouse_dy = 0;

		if (mousex <= 0)
			mousex = 0;
		if (mousex > video_inst->width)
			mousex = video_inst->width - 1;

		if (mousey <= 0)
			mousey = 0;
		if (mousey > video_inst->height)
			mousey = video_inst->height - 1;

		set_pixel (video_inst, mousex, mousey, 0xFFFFFFFF);
		swap_buffers (video_inst, 0, 0, 320, 200);
	}
}
