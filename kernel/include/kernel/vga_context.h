#ifndef _KERNEL_VGA_CONTEXT_H
#define _KERNEL_VGA_CONTEXT_H

#include <kernel/context.h>
#include <kernel/video.h>
#include <kernel/device.h>

#include <arch/i386/portio.h>

#define VGA_MEMORY 0xA0000

class VGAContext : public Context, public Driver::Device {
public:
  VGAContext (int id, const char *name);

  inline void SwapBuffers ();
private:
  void setPalette ();
  void writeVGARegs (uint8_t *regs);

  inline uint8_t *getVGAAddress () const { return (uint8_t *) VGA_MEMORY; };

  inline void setPaletteIndex (uint8_t index) { outb (0x03c8, index); };

  inline void setPaletteColor (uint8_t red, uint8_t green, uint8_t blue);
  inline void setColorAtIndex (uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
};

#endif
