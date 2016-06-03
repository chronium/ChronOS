#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

#include <kernel/video.h>

namespace Driver {

class Terminal : public Video {
public:
  Terminal (int id, const char *name);
  ~Terminal ();

  size_t Read (void *buffer, size_t len, uint32_t address);
  size_t Write (const void *buffer, size_t len, uint32_t address);

  void SetCursor ();

  void PutC (char c);
  void Write (const char *data, size_t size);
  void WriteS (const char *data);

  void Newline ();
  void Scroll ();
  void Clear ();

  void PutEntry (char c);
  void PutEntry (char c, size_t x, size_t y);

  void *GetAddress () const { return (void *) 0xB8000; }

  inline uint8_t GetColor () const { return fgColor | bgColor << 4; }
private:
  uint8_t fgColor;
  uint8_t bgColor;

  size_t x;
  size_t y;
};
  extern "C" Terminal *terminal;
}

#endif
