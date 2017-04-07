	.data
c:	.word 0

	.text
	.global main
main:

# Prologue begins
sw $ra, 0($sp)	#save the return address
sw $fp, -4($sp)	#save the frame pointer
sub $sp, $sp, 8	#update the frame pointer

sub $sp, $sp 8	#Make space for the locals
#Prologue ends
	j epilouge_main
# Epilogue begins
epilouge_main:
add $sp, $sp, 8
lw $fp, -4($sp)
lw $ra, 0($sp)
jr	$31	# Jump back to the called procedure
	.text
	.global one
one:

# Prologue begins
sw $ra, 0($sp)	#save the return address
sw $fp, -4($sp)	#save the frame pointer
sub $sp, $sp, 8	#update the frame pointer

sub $sp, $sp 8	#Make space for the locals
#Prologue ends
	li $v0, 10
	sw $v0, 0($fp)
	lw $v0, 0($fp)
	move $v1, $v0
	j epilouge_one
# Epilogue begins
epilouge_one:
add $sp, $sp, 8
lw $fp, -4($sp)
lw $ra, 0($sp)
jr	$31	# Jump back to the called procedure
	.text
	.global two
two:

# Prologue begins
sw $ra, 0($sp)	#save the return address
sw $fp, -4($sp)	#save the frame pointer
sub $sp, $sp, 8	#update the frame pointer

sub $sp, $sp 8	#Make space for the locals
#Prologue ends
	lw $v0, 0($fp)
	move $v1, $v0
	j epilouge_two
# Epilogue begins
epilouge_two:
add $sp, $sp, 8
lw $fp, -4($sp)
lw $ra, 0($sp)
jr	$31	# Jump back to the called procedure
