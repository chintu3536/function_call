	.data
A:	.word 0
F1:	.space 8
F2:	.space 8
__string__0:	.asciiz	"\n"

	.text
	.globl faddfunc
faddfunc:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 8
#Prologue ends
	l.d $f2, 8($fp)
	s.d $f2, -8($fp)
	l.d $f2, 0($fp)
	li.d $f4, 0
	seq $v0, $f2, $f4
	beq $v0, $zero, label0
	l.d $f2, 8($fp)
	mov.d $f0, $f2
	j epilouge_faddfunc
	j label1

label0:    	

label1:    	
	li.d $f2, 1
	l.d $f4, 0($fp)
	li.d $f6, 1
	sub.d $f8, $f4, $f6
	s.d $f8, -8($sp)
	l.d $f4, -8($fp)
	s.d $f4, -16($sp)
	sub $sp, $sp, 16
	jal faddfunc
	add $sp, $sp, 16
	mov.d $f4, $f0
	add.d $f6, $f2, $f4
	mov.d $f0, $f6
	j epilouge_faddfunc
# Epilogue begins
epilouge_faddfunc:
	add $sp, $sp, 8
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
	.text
	.globl iaddfunc
iaddfunc:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 4
#Prologue ends
	lw $v0, 8($fp)
	sw $v0, -4($fp)
	lw $v0, 4($fp)
	li $t0, 0
	seq $t1, $v0, $t0
	beq $t1, $zero, label2
	lw $v0, 8($fp)
	move $v1, $v0
	j epilouge_iaddfunc
	j label3

label2:    	

label3:    	
	li $v0, 1
	lw $t0, 4($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -4($sp)
	lw $t0, -4($fp)
	sw $t0, -8($sp)
	sub $sp, $sp, 8
	jal iaddfunc
	add $sp, $sp, 8
	move $t0, $v1
	add $t1, $v0, $t0
	move $v1, $t1
	j epilouge_iaddfunc
# Epilogue begins
epilouge_iaddfunc:
	add $sp, $sp, 4
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
	.text
	.globl main
main:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 36
#Prologue ends
	li.d $f2, 9.5
	s.d $f2, F1
	li $v0, 1
	sw $v0, -4($fp)
	li.d $f2, 3.5
	s.d $f2, -20($fp)
	li $v0, 2
	sw $v0, -8($fp)
	li.d $f2, 4.5
	s.d $f2, -28($fp)
	li $v0, 3
	sw $v0, -12($fp)
	li.d $f2, 5.5
	s.d $f2, -36($fp)
	li.d $f2, 5
	s.d $f2, -8($sp)
	li.d $f2, 4
	s.d $f2, -8($sp)
	li.d $f2, 3
	s.d $f2, -8($sp)
	li.d $f2, 2
	s.d $f2, -8($sp)
	li.d $f2, 1
	li.d $f4, 1
	s.d $f4, -8($sp)
	l.d $f4, -28($fp)
	s.d $f4, -16($sp)
	sub $sp, $sp, 16
	jal faddfunc
	add $sp, $sp, 16
	mov.d $f4, $f0
	add.d $f6, $f2, $f4
	s.d $f6, -16($sp)
	sub $sp, $sp, 16
	jal faddfunc
	add $sp, $sp, 16
	mov.d $f2, $f0
	s.d $f2, -16($sp)
	sub $sp, $sp, 16
	jal faddfunc
	add $sp, $sp, 16
	mov.d $f2, $f0
	s.d $f2, -16($sp)
	sub $sp, $sp, 16
	jal faddfunc
	add $sp, $sp, 16
	mov.d $f2, $f0
	li.d $f4, 5
	sub.d $f6, $f2, $f4
	s.d $f6, -16($sp)
	sub $sp, $sp, 16
	jal faddfunc
	add $sp, $sp, 16
	mov.d $f2, $f0
	addi $sp, $sp, -4
	sw $v0, 0($sp)
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	addi $sp, $sp, -8
	s.d $f12, 0($sp)
	mov.d $f12, $f2
	li $v0 3
	syscall
	l.d $f12, 0($sp)
	addi $sp, $sp, 8
	lw $a0, 0($sp)
	addi $sp, $sp, 4
	lw $v0, 0($sp)
	addi $sp, $sp, 4
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
	li $v0, 5
	sw $v0, -4($sp)
	li $v0, 4
	sw $v0, -4($sp)
	li $v0, 3
	sw $v0, -4($sp)
	li $v0, 2
	sw $v0, -4($sp)
	li $v0, 3
	li $t0, 1
	sw $t0, -4($sp)
	lw $t0, -8($fp)
	sw $t0, -8($sp)
	sub $sp, $sp, 8
	jal iaddfunc
	add $sp, $sp, 8
	move $t0, $v1
	add $t1, $v0, $t0
	sw $t1, -8($sp)
	sub $sp, $sp, 8
	jal iaddfunc
	add $sp, $sp, 8
	move $v0, $v1
	sw $v0, -8($sp)
	sub $sp, $sp, 8
	jal iaddfunc
	add $sp, $sp, 8
	move $v0, $v1
	sw $v0, -8($sp)
	sub $sp, $sp, 8
	jal iaddfunc
	add $sp, $sp, 8
	move $v0, $v1
	li $t0, 5
	sub $t1, $v0, $t0
	sw $t1, -8($sp)
	sub $sp, $sp, 8
	jal iaddfunc
	add $sp, $sp, 8
	move $v0, $v1
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
	li $v0, 0
	move $v1, $v0
	j epilouge_main
# Epilogue begins
epilouge_main:
	add $sp, $sp, 36
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	li $v0, 10
	syscall
