	.file	"intro_sync.c"
	.intel_syntax noprefix
	.text
	.globl	th_inc_func
	.type	th_inc_func, @function
th_inc_func:
.LFB52:
	.cfi_startproc
	endbr64
	mov	edx, 50000000
.L2:
	mov	eax, DWORD PTR x[rip]
	add	eax, 1
	mov	DWORD PTR x[rip], eax
	sub	edx, 1
	jne	.L2
	mov	eax, 0
	ret
	.cfi_endproc
.LFE52:
	.size	th_inc_func, .-th_inc_func
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Creating thread %d...\n"
.LC1:
	.string	"pthread_create failed"
.LC2:
	.string	"Finished thread %d.\n"
.LC3:
	.string	"pthread_join failed"
.LC4:
	.string	"\nFinal value of x = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB53:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	sub	rsp, 32
	.cfi_def_cfa_offset 48
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 24[rsp], rax
	xor	eax, eax
	mov	edx, 0
	lea	rsi, .LC0[rip]
	mov	edi, 2
	call	__printf_chk@PLT
	mov	rdi, rsp
	mov	ecx, 0
	lea	rdx, th_inc_func[rip]
	mov	esi, 0
	call	pthread_create@PLT
	mov	ebx, eax
	test	eax, eax
	jne	.L5
	mov	edx, 1
	lea	rsi, .LC0[rip]
	mov	edi, 2
	mov	eax, 0
	call	__printf_chk@PLT
	lea	rdi, 8[rsp]
	mov	ecx, 0
	lea	rdx, th_inc_func[rip]
	mov	esi, 0
	call	pthread_create@PLT
	mov	ebx, eax
	test	eax, eax
	jne	.L5
	mov	esi, 0
	mov	rdi, QWORD PTR [rsp]
	call	pthread_join@PLT
	mov	ebx, eax
	test	eax, eax
	jne	.L7
	mov	edx, 0
	lea	rsi, .LC2[rip]
	mov	edi, 2
	mov	eax, 0
	call	__printf_chk@PLT
	mov	esi, 0
	mov	rdi, QWORD PTR 8[rsp]
	call	pthread_join@PLT
	mov	ebx, eax
	test	eax, eax
	jne	.L7
	mov	edx, 1
	lea	rsi, .LC2[rip]
	mov	edi, 2
	mov	eax, 0
	call	__printf_chk@PLT
	mov	edx, DWORD PTR x[rip]
	lea	rsi, .LC4[rip]
	mov	edi, 2
	mov	eax, 0
	call	__printf_chk@PLT
	mov	rax, QWORD PTR 24[rsp]
	sub	rax, QWORD PTR fs:40
	jne	.L11
	mov	eax, 0
	add	rsp, 32
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
.L5:
	.cfi_restore_state
	call	__errno_location@PLT
	mov	DWORD PTR [rax], ebx
	lea	rdi, .LC1[rip]
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L7:
	call	__errno_location@PLT
	mov	DWORD PTR [rax], ebx
	lea	rdi, .LC3[rip]
	call	perror@PLT
	mov	edi, 1
	call	exit@PLT
.L11:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE53:
	.size	main, .-main
	.globl	x
	.bss
	.align 4
	.type	x, @object
	.size	x, 4
x:
	.zero	4
	.globl	NUM_ITERATIONS
	.section	.rodata
	.align 4
	.type	NUM_ITERATIONS, @object
	.size	NUM_ITERATIONS, 4
NUM_ITERATIONS:
	.long	50000000
	.globl	NUM_THREADS
	.align 4
	.type	NUM_THREADS, @object
	.size	NUM_THREADS, 4
NUM_THREADS:
	.long	2
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
