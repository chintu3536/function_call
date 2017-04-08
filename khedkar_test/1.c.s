
	.data
c:	.word 0

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
	li $v0, 2
	sw $v0, 0($sp)
	li $v0, 1
	sw $v0, -4($sp)
	lw $v0, -4($fp)
	sw $v0, -8($sp)
	sub $sp, $sp, 12
	jal one
	add $sp, $sp, 12
	move $v0, $v1
	sw $v0, 0($fp)
	li $v0, 2
	sw $v0, c
	j epilogue_main

# Epilogue Begins
epilogue_main:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl one		# The following is the code (as oppose to data)
one:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 16		# Make space for the locals
# Prologue ends

	lw $v0, 12($fp)
	sw $v0, 0($fp)
	lw $v0, 12($fp)
	sw $v0, 12($fp)
	lw $v0, 12($fp)
	move $v1, $v0
	j epilogue_one

# Epilogue Begins
epilogue_one:
	add $sp, $sp, 16
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends


	.text 			# The .text assembler directive indicates
	.globl two		# The following is the code (as oppose to data)
two:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $ra, 0($sp)		# Save the return address
	sw $fp, -4($sp)		# Save the frame pointer
	sub $fp, $sp, 8		# Update the frame pointer

	sub $sp, $sp, 12		# Make space for the locals
# Prologue ends

	lw $v0, 12($fp)
	move $v1, $v0
	j epilogue_two

# Epilogue Begins
epilogue_two:
	add $sp, $sp, 12
	lw $fp, -4($sp)  
	lw $ra, 0($sp)   
	jr        $31		# Jump back to the called procedure
# Epilogue Ends

