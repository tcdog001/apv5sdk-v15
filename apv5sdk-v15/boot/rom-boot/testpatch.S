	.globl patchmain
	.text

patchmain:
	addiu	$sp,	-4	/* space for $ra */
	sw	$ra,	0($sp)
	j	1f
	nop
str:
	.string "rom patch test\n"
1:
	la	$a0,	str
	li	$t0,	0x9f00114c	// serial_puts
	jalr	$t0
	nop
	lw	$ra,	0($sp)
	addiu	$sp,	4
	jr	$ra
	nop
2:
	j	2b
	nop
