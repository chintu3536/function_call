	.data
c:	.word 0
__string__0:	.asciiz	"what do you mean??\n"

	.text
	.globl main
main:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 8
#Prologue ends
	li $v0, 1
	sw $v0, -8($fp)
	lw $v0, -8($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 4
	jal one
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, -8($fp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -8
	s.d $f12, 0($sp)
	la $a0, __string__0
	li $v0 4
	syscall
	l.d $f12, 0($sp)
	addi $sp, $sp, 8
	lw $a0, 0($sp)
	addi $sp, $sp, 4
	lw $v0, 0($sp)
	addi $sp, $sp, 4
	li $v0, 2
	sw $v0, c
	j epilouge_main
# Epilogue begins
epilouge_main:
	add $sp, $sp, 8
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	li $v0, 10
	syscall
	.text
	.globl one
one:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 8
#Prologue ends
	lw $v0, 8($fp)
	sw $v0, -4($fp)
	lw $v0, 8($fp)
	sw $v0, 8($fp)
	lw $v0, 8($fp)
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -8
	s.d $f12, 0($sp)
	move $a0, $v0
	li $v0 1
	syscall
	l.d $f12, 0($sp)
	addi $sp, $sp, 8
	lw $a0, 0($sp)
	addi $sp, $sp, 4
	lw $v0, 0($sp)
	addi $sp, $sp, 4
	lw $v0, 8($fp)
	move $v1, $v0
	j epilouge_one
# Epilogue begins
epilouge_one:
	add $sp, $sp, 8
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
	.text
	.globl two
two:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 4
#Prologue ends
	lw $v0, 8($fp)
	move $v1, $v0
	j epilouge_two
# Epilogue begins
epilouge_two:
	add $sp, $sp, 4
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
