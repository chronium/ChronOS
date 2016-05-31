#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

#include <kernel/video.h>

struct textmode *term_init (struct video *);
void term_putc (char);
void term_write (const char *, size_t);
void term_writes (const char *);
void term_set_cursor (size_t, size_t);

size_t term_write_dev (void *, const void *, size_t, uint32_t);

#endif
