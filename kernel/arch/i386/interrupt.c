#include <stdint.h>
#include <stdio.h>

#include <arch/i386/pic.h>
#include <arch/i386/interrupt.h>
#include <arch/i386/exception.h>

void isr_handler (struct interrupt_context *int_ctx) {
	handle_exception (int_ctx);
}

void irq_handler (struct interrupt_context *int_ctx) {
	uint8_t irq = int_ctx->int_no - 32;

	(void) int_ctx;

	if (irq == 7 && !(pic_read_isr () & (1 << 7))) 
		return;
	if (irq == 15 && !(pic_read_isr () & (1 << 15)))
		return;

	if (8 <= irq)
		pic_eoi_slave ();
	pic_eoi_master ();
}

void interrupt_handler (struct interrupt_context *int_ctx) {
	if (int_ctx->int_no < 32)
		isr_handler (int_ctx);
	else if (32 <= int_ctx->int_no && int_ctx->int_no < 32 + 16)
		irq_handler (int_ctx);
}
