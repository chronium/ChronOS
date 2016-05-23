.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
  movl $stack_top, %esp

  movl $tss, %ecx
	movw %cx, gdt + 0x28 + 2
	shrl $16, %ecx
	movb %cl, gdt + 0x28 + 4
	shrl $8, %ecx
	movb %cl, gdt + 0x28 + 7

  subl $6, %esp
	movw gdt_size_minus_one, %cx
	movw %cx, 0(%esp)
	movl $gdt, %ecx
	movl %ecx, 2(%esp)
	lgdt 0(%esp)
	addl $6, %esp

  push $0x08
	push $1f
	retf
1:

  movw $0x10, %cx
  movw %cx, %ds
  movw %cx, %es
  movw %cx, %fs
  movw %cx, %gs
  movw %cx, %ss

  movw $(0x28 | 0x3), %cx
	ltr %cx

  push %ebx
  call kearly
  call _init
  call kmain
  cli
.Lhang:
  hlt
  jmp .Lhang
.size _start, . - _start
