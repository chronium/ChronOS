#ifndef _ARCH_I386_EXCEPTION_H
#define _ARCH_I386_EXCEPTION_H

#include <arch/i386/interrupt.h>

void handle_exception (struct interrupt_context *);

#endif
