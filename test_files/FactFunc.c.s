	.data

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
	li $v0, 5
	sw $v0, -4($fp)
	lw $v0, -8($fp)
	sw $v0, -4($sp)
	lw $v0, -4($fp)
	sw $v0, -8($sp)
	sub $sp, $sp, 8
	jal nonrecurfn
	add $sp, $sp, 8
	lw $v0, -4($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 4
	jal recurfn
	add $sp, $sp, 4
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
	.globl nonrecurfactorial
nonrecurfactorial:

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
	li $v0, 1
	sw $v0, -4($fp)
	j label1

label0:    	
	lw $v0, -8($fp)
	lw $t0, -4($fp)
	mul $t2, $v0, $t0
	sw $t2, -8($fp)
	lw $v0, -4($fp)
	li $t0, 1
	add $t2, $v0, $t0
	sw $t2, -4($fp)

label1:    	
	lw $v0, -4($fp)
	lw $t0, 8($fp)
	sle $t1, $v0, $t0
	bne $t1, $zero, label0
	li.d $f2, 1.00
	mov.d $f0, $f2
	j epilouge_nonrecurfactorial
# Epilogue begins
epilouge_nonrecurfactorial:
	add $sp, $sp, 8
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
	.text
	.globl nonrecurfn
nonrecurfn:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 8
#Prologue ends
	lw $v0, 8($fp)
	li $t0, 0
	slt $t1, $v0, $t0
	beq $t1, $zero, label2
	j label3

label2:    	
	lw $v0, 8($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 4
	jal nonrecurfactorial
	add $sp, $sp, 4
	mov.d $f2, $f0
	s.d $f2, -8($fp)

label3:    	
	j epilouge_nonrecurfn
# Epilogue begins
epilouge_nonrecurfn:
	add $sp, $sp, 8
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
	.text
	.globl recurfactorial
recurfactorial:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 8
#Prologue ends
	lw $v0, 8($fp)
	li $t0, 0
	seq $t1, $v0, $t0
	beq $t1, $zero, label4
	li $v0, 1
	sw $v0, -4($fp)
	j label5

label4:    	
	lw $v0, 8($fp)
	li $t0, 1
	sub $t2, $v0, $t0
	sw $t2, -4($sp)
	sub $sp, $sp, 4
	jal recurfactorial
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, -4($fp)
	lw $v0, 8($fp)
	lw $t0, -4($fp)
	mul $t2, $v0, $t0
	sw $t2, -4($fp)

label5:    	
	lw $v0, -4($fp)
	move $v1, $v0
	j epilouge_recurfactorial
# Epilogue begins
epilouge_recurfactorial:
	add $sp, $sp, 8
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
	.text
	.globl recurfn
recurfn:

#Prologue begins
	sub $sp, $sp, 4	# first make space on stack
	sw $ra, 0($sp)
	sub $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	sub $sp, $sp, 4
#Prologue ends
	lw $v0, 8($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 4
	jal recurfactorial
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, -4($fp)
	j epilouge_recurfn
# Epilogue begins
epilouge_recurfn:
	add $sp, $sp, 4
	lw $fp, 0($sp)
	add $sp, $sp, 4
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra	#Jump back to the called procedure
