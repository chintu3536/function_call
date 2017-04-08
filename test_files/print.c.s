	.data
__string__0:	.asciiz	"asl"

	.text
	.globl main
main:

# Prologue begins
sw $ra, 0($sp)	#save the return address
sw $fp, -4($sp)	#save the frame pointer
sub $sp, $sp, 8	#update the frame pointer

sub $sp, $sp, 8	#Make space for the locals
#Prologue ends
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
	j epilouge_main
# Epilogue begins
epilouge_main:
add $sp, $sp, 8
lw $fp, -4($sp)
lw $ra, 0($sp)
jr	$31	# Jump back to the called procedure
