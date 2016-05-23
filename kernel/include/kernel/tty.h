#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void term_init (void);
void term_putc (char c);
void term_write (const char *data, size_t size);
void term_writes (const char *data);

#endif