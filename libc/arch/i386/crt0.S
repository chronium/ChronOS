.section .start

.global _start
.type _start, @function
_start:
	xorl %ebp, %ebp
	pushl %ebp
	pushl %ebp
	movl %esp, %ebp

	push $0 # TODO: argc
	push $0 # TODO: argv
	push $0 # TODO: envp
	call __init_libc
	addl $12, %esp

#if !defined (__HAS_NO_CRT_INIT)
	call _init
#endif

	push $0 # TODO: argc
	push $0 # TODO: argv
	push $0 # TODO: envp
	call main
	addl $12, %esp

	push %eax
	call exit
.size _start, . - _start