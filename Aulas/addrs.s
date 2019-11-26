	.file	"addrs.c"
	.text
	.section	.rodata
.LC0:
	.string	"array base addr:\t %p\n"
.LC1:
	.string	"element %d address:\t %p\n"
.LC2:
	.string	"2nd array value:\t %d\n"
	.align 8
.LC3:
	.string	"-------------------------------"
.LC4:
	.string	"base address of struct:\t %p\n"
.LC5:
	.string	"address of number:\t %p\n"
.LC6:
	.string	"address of name:\t %p\n"
.LC7:
	.string	"address of 2nd student:\t %p\n"
.LC8:
	.string	"map memory:"
.LC9:
	.string	"addr:\t%p\tvalue:\t%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$208, %rsp
	movl	%edi, -196(%rbp)
	movq	%rsi, -208(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$46074, -144(%rbp)
	movabsq	$2337197174051531090, %rax
	movabsq	$7018979184860094797, %rdx
	movq	%rax, -140(%rbp)
	movq	%rdx, -132(%rbp)
	movq	$117, -124(%rbp)
	movq	$0, -116(%rbp)
	movq	$0, -108(%rbp)
	movq	$0, -100(%rbp)
	movq	$0, -92(%rbp)
	movq	$0, -84(%rbp)
	movq	$0, -76(%rbp)
	movq	$0, -68(%rbp)
	movq	$0, -60(%rbp)
	movq	$0, -52(%rbp)
	movq	$0, -44(%rbp)
	movq	$0, -36(%rbp)
	movq	$0, -28(%rbp)
	movq	$0, -20(%rbp)
	movl	$123, -160(%rbp)
	movl	$213, -156(%rbp)
	movl	$12323, -152(%rbp)
	movl	$312, -148(%rbp)
	leaq	-160(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, -180(%rbp)
	jmp	.L2
.L3:
	leaq	-160(%rbp), %rax
	movl	-180(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rax, %rdx
	movl	-180(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -180(%rbp)
.L2:
	movl	-180(%rbp), %eax
	cmpl	$3, %eax
	jbe	.L3
	movl	-156(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC3(%rip), %rdi
	call	puts@PLT
	leaq	-144(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-144(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-144(%rbp), %rax
	addq	$4, %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-144(%rbp), %rax
	movq	%rax, -168(%rbp)
	movq	-168(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	leaq	-160(%rbp), %rax
	movq	%rax, -176(%rbp)
.L4:
	movq	-176(%rbp), %rax
	movl	(%rax), %edx
	movq	-176(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC9(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$4, -176(%rbp)
	jmp	.L4
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.0"
	.section	.note.GNU-stack,"",@progbits
