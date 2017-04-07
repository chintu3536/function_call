
	.data
__string__0:	.asciiz	"asl"


	.text
	.globl main
main:
	sub $sp $sp 8
	sw $ra 4($sp)
	sw $fp 0($sp)
	add $fp $sp 8

	sub $sp, $sp, 8
	sw $v0, 0($sp)
	sw $a0, 4($sp)
	la $a0, __string__0
	li $v0, 4
	syscall 
	sw $a0, 4($sp)
	sw $v0, 0($sp)
	add $sp, $sp, 8
	j __ret__main

__ret__main:
	lw $fp 0($sp)
	lw $ra 4($sp)
	add $sp $sp 8
	jr $ra
	.end main
