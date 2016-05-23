.macro isr id
  .global isr\id
  .type isr\id, @function
  isr\id:
    pushl $0
    pushl $\id
    jmp interrupt_handler_prep
  .size isr\id, . - isr\id
.endm

.macro isr_err id index
  .global isr\id
  .type isr\id, @function
  isr\id:
    pushl $\id
    jmp interrupt_handler_prep
  .size isr\id, . - isr\id
.endm

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_err 8
isr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr 15
isr 16
isr_err 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr_err 30
isr 31
isr 32

.macro irq id
  .global irq\id
  .type irq\id, @function
  irq\id:
    pushl $0
    pushl $(\id + 32)
    jmp interrupt_handler_prep
  .size irq\id, . - irq\id
.endm

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15

.type interrupt_handler_prep, @function
interrupt_handler_prep:
  pushl %eax
  pushl %ecx
  pushl %edx
  pushl %ebx
  pushl %ebp
  pushl %esi
  pushl %edi
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs

  movl $0x10, %ebp
  movl %ebp, %ds
  movl %ebp, %es
  movl %ebp, %fs
  movl %ebp, %gs

  movl %cr2, %ebp
  subl $4, %esp
  andl $0xFFFFFFF0, %esp
  movl %ebp, (%esp)
  call interrupt_handler
  movl %ebx, %esp

  popl %ebp
  movl %ebp, %cr2

  popl %gs
  popl %fs
  popl %es
  popl %ds
  popl %edi
	popl %esi
	popl %ebp
	popl %ebx
	popl %edx
	popl %ecx
	popl %eax

	addl $8, %esp
	iret
.size interrupt_handler_prep, . - interrupt_handler_prep
