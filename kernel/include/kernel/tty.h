#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void term_init (void);
void term_putc (char);
void term_write (const char *, size_t);
void term_writes (const char *);
void term_set_cursor (size_t, size_t);

#endif
