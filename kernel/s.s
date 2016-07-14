	.file	"start.c"
	.comm	gdt_ptr,6,1
	.comm	gdt,1024,32
	.section	.rodata
	.align 8
.LC0:
	.string	"------------Welcom to use MuteOS !!!-------------"
	.text
	.globl	cstart
	.type	cstart, @function
cstart:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC0, %edi
	call	print
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	cstart, .-cstart
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
