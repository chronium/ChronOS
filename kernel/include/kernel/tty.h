#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

#include <kernel/video.h>

typedef struct textmode {
  uint16_t *address;

  uint8_t fg_color;
  uint8_t bg_color;

  size_t x;
  size_t y;

  struct video *video;
} textmode_t;

extern struct video *textscreen;


textmode_t *term_init (struct video *);
void term_putc (textmode_t *tty, char);
#ifdef __cplusplus
extern "C"
#endif
void term_write (textmode_t *tty, const char *, size_t);
void term_writes (textmode_t *tty, const char *);
void term_set_cursor (textmode_t *tty, size_t, size_t);

#ifdef __cplusplus
extern "C"
#endif
textmode_t *get_textmode ();

size_t term_write_dev (void *, const void *, size_t, uint32_t);

#endif
