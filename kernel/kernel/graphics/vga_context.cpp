#include <kernel/vga_context.h>
#include <kernel/device.h>

#include <arch/i386/portio.h>

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

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

VGAContext::VGAContext (const char *name) :
  Context (320, 200),
  Driver::Device (name, DeviceType::BlockDevice) {
  this->writeVGARegs (vga_320x200x256);
  this->setPalette ();
  this->Clear ();
  this->SwapBuffers ();
}

void VGAContext::setPalette () {
  this->setPaletteIndex (0);

  for (size_t i = 0; i < 256; i++) {
   size_t red = ((i >> 5) & 7) * 63 / 7;
   size_t green = ((i >> 2) & 7) * 63 / 7;
   size_t blue = (i & 3) * 63 / 3;

   this->setPaletteColor ((uint8_t) red, (uint8_t) green, (uint8_t) blue);
  }
}

void VGAContext::writeVGARegs (uint8_t *regs) {
	/* write MISCELLANEOUS reg */
  outb (VGA_MISC_WRITE, *regs);
  regs++;

  /* write SEQUENCER regs */
  for (int i = 0; i < VGA_NUM_SEQ_REGS; i++) {
    outb (VGA_SEQ_INDEX, i);
    outb (VGA_SEQ_DATA, *regs);
    regs++;
  }

  /* unlock CRTC registers */
  outb (VGA_SEQ_INDEX, 0x03);
  outb (VGA_SEQ_DATA, inb (VGA_CRTC_DATA) | 0x80);
  outb (VGA_SEQ_INDEX, 0x11);
  outb (VGA_SEQ_DATA, inb (VGA_CRTC_DATA) & ~0x80);

  /* make sure they remain unlocked */
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;

  /* write CRTC regs */
  for (int i = 0; i < VGA_NUM_CRTC_REGS; i++) {
    outb (VGA_CRTC_INDEX, i);
    outb (VGA_CRTC_DATA, *regs);
    regs++;
  }

  /* write GRAPHICS CONTROLLER regs */
  for (int i = 0; i < VGA_NUM_GC_REGS; i++) {
    outb (VGA_GC_INDEX, i);
    outb (VGA_GC_DATA, *regs);
    regs++;
  }

  /* write ATTRIBUTE CONTROLLER regs */
  for (int i = 0; i < VGA_NUM_AC_REGS; i++) {
    (void) inb (VGA_INSTAT_READ);
    outb (VGA_AC_INDEX, i);
    outb (VGA_AC_WRITE, *regs);
    i++;
  }

  /* lock 16-color palette and unblank display */
  (void) inb (VGA_INSTAT_READ);
  outb (VGA_AC_INDEX, 0x20);
}

#pragma GCC push_options
#pragma GCC optimize("O3")
inline void VGAContext::SwapBuffers () {
  for (int y = 0; y < this->GetHeight (); y++)
    for (int x = 0; x < this->GetWidth (); x++) {
      uint32_t c = this->GetBuffer ()[x + y * this->GetWidth ()];

      uint8_t red = ((c >> 16) & 0xFF) / 32;
      uint8_t green = ((c >> 8) & 0xFF) / 32;
      uint8_t blue = (c & 0xFF) / 64;

			this->getVGAAddress()[x + y * this->GetWidth ()] = (red << 5) | (green << 2) | blue;
    }
}
#pragma GCC pop_options

inline void VGAContext::setPaletteColor (uint8_t red, uint8_t green, uint8_t blue) {
  outb (0x03c9, red);
  outb (0x03c9, green);
  outb (0x03c9, blue);
}

inline void VGAContext::setColorAtIndex (uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
  this->setPaletteIndex (index);
  this->setPaletteColor (red, green, blue);
}
