#include <stdbool.h>
#include <stdio.h>

#include <arch/i386/exception.h>

void handle_exception (struct interrupt_context *int_ctx) {
	asm volatile ("cli");

	printf("Exception: %s\n", exceptions[int_ctx->int_no]);

	while (true) asm ("hlt");
}
