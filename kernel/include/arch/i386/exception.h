#ifndef _ARCH_I386_EXCEPTION_H
#define _ARCH_I386_EXCEPTION_H

#include <arch/i386/interrupt.h>

static char *exceptions[] = {
	"Divide by 0",
	"Single step",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bounds check",
	"Invalid opcode",
	"Device not available",
	"Double fault",
	"Coprocessor Segment Overrun",
	"Invalid Task State Segment",
	"Segment Not Present",
	"Stack Fault Exception",
	"General Protection Fault",
	"Page Fault",
	"Unassigned",
	"x87 FPU Error",
	"Alignment Check",
	"Machine Check",
	"SIMD FPU Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

void handle_exception (struct interrupt_context *);

#endif
