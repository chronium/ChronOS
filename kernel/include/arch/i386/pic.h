#ifndef _ARCH_I386_PIC_H
#define _ARCH_I386_PIC_H

#include <stdint.h>

#include <arch/i386/portio.h>

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0
#define PIC_COMMAND 0x00
#define PIC_DATA 0x01
#define PIC_CMD_ENDINTR 0x20
#define PIC_ICW1_ICW4 0x01
#define PIC_ICW1_SINGLE 0x02
#define PIC_ICW1_INTERVAL4 0x04
#define PIC_ICW1_LEVEL 0x08
#define PIC_CMD_INIT 0x10
#define PIC_MODE_8086 0x01
#define PIC_MODE_AUDO 0x02
#define PIC_MODE_BUF_SLAVE 0x08
#define PIC_MODE_BUF_MASTRE 0x0C
#define PIC_MODE_SFNM 0x10
#define PIC_READ_IRR 0x0A
#define PIC_READ_ISR 0x0B

typedef struct regs {
  uint32_t ds;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t int_no;
  uint32_t err_code;
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t useresp;
  uint32_t ss;
} regs_t;

typedef void (*inthandler_t) (regs_t *);

int request_isr (int, inthandler_t);
int free_isr (int);

int request_irq (int, inthandler_t);
int free_irq (int);

void pic_remap ();

static inline void pic_eoi_master () {
  outb (PIC_MASTER, PIC_CMD_ENDINTR);
}

static inline void pic_eoi_slave () {
  outb (PIC_SLAVE, PIC_CMD_ENDINTR);
}

#endif
