
	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends

	li $v0, 1
	sw $v0, -4($fp)
	li $v0, 5
	sw $v0, 0($fp)
	lw $v0, -4($fp)
	sw $v0, 0($sp)
	lw $v0, 0($fp)
	sw $v0, -4($sp)
	sub $sp, $sp, 8
	jal nonrecurfn
	add $sp, $sp, 8
	lw $v0, 0($fp)
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal recurfn
	add $sp, $sp, 4
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl nonrecurfactorial		# The following is the code (as oppose to data)
nonrecurfactorial:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends

	li $v0, 1
	sw $v0, -4($fp)
	li $v0, 1
	sw $v0, 0($fp)
	j label1

label0:    	
	lw $v0, -4($fp)
	lw $t0, 0($fp)
	mul $t2, $v0, $t0
	sw $t2, -4($fp)
	lw $v0, 0($fp)
	li $t0, 1
	add $t2, $v0, $t0
	sw $t2, 0($fp)

label1:    	
	lw $v0, 0($fp)
	lw $t0, 12($fp)
	sle $t1, $v0, $t0
	bne $t1, $zero, label0 
	li.d $f2, 1.00
	mov.d $f0, $f2
	j epilogue_nonrecurfactorial

# Epilogue Begins
epilogue_nonrecurfactorial:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl nonrecurfn		# The following is the code (as oppose to data)
nonrecurfn:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends

	lw $v0, 12($fp)
	li $t0, 0
	slt $t1, $v0, $t0
	beq $t1, $zero, label2 
	j label3

label2:    	
	lw $v0, 12($fp)
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal nonrecurfactorial
	add $sp, $sp, 4
	mov.d $f2, $f0
	s.d $f2, 0($fp)

label3:    	
	j epilogue_nonrecurfn

# Epilogue Begins
epilogue_nonrecurfn:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl recurfactorial		# The following is the code (as oppose to data)
recurfactorial:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends

	lw $v0, 12($fp)
	li $t0, 0
	seq $t1, $v0, $t0
	beq $t1, $zero, label4 
	li $v0, 1
	sw $v0, 0($fp)
	j label5

label4:    	
	lw $v0, 12($fp)
	li $t0, 1
	sub $t2, $v0, $t0
	sw $t2, 0($sp)
	sub $sp, $sp, 4
	jal recurfactorial
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, 0($fp)
	lw $v0, 12($fp)
	lw $t0, 0($fp)
	mul $t1, $v0, $t0
	sw $t1, 0($fp)

label5:    	
	lw $v0, 0($fp)
	move $v1, $v0
	j epilogue_recurfactorial

# Epilogue Begins
epilogue_recurfactorial:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl recurfn		# The following is the code (as oppose to data)
recurfn:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 12		# Make space for the locals
# Prologue ends

	lw $v0, 12($fp)
	sw $v0, 0($sp)
	sub $sp, $sp, 4
	jal recurfactorial
	add $sp, $sp, 4
	move $v0, $v1
	sw $v0, 0($fp)
	j epilogue_recurfn

# Epilogue Begins
epilogue_recurfn:
	add $sp, $sp, 12
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

