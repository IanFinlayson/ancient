# The following functions make up the Mandrill standard library
# They were hand written in assembly and supplied by the compiler

# printInt:int( num:int ) -> ($v0)
printInt:
	lw $a0, 0($sp)
	li $v0, 1
	syscall
	move $v0, $a0
	jr $ra

# newLine:int( )
newLine:
	la $a0, endl
	li $v0, 4
	syscall
	li $v0, 0
	jr $ra

# This function can only be called internally
# it is how the ^ opertor is implemented
powMAND:
	li $v0, 1
powMANDTOP:
	beqz $a1, powMANDEND
	mul $v0, $v0, $a0
	addi $a1, $a1, -1
	b powMANDTOP
powMANDEND:
	jr $ra


# readInt:int( ) -> ($v0)
readInt:
	li $v0, 5
	syscall
	jr $ra

# printChar:char( c:char ) -> ($v0)
printChar:
	addi $sp, $sp, -1
	li $a0, 0
	sb $a0, 0($sp)
	addi $a0, $sp, 1
	li $v0, 4
	syscall
	addi $sp, $sp, 1
	lb $v0, 0($sp)
	jr $ra


# These next two do nothing at all
# They serve as a way to tell compiler
# that you know you are treating chars as
# integral (if you want to subtract for whatever reason)
# int_of_char:int( c:char ) -> ($v0)
int_of_char:
	lw $v0, 0($sp)
	jr $ra

# char_of_int:char( x:int ) -> ($v0)
char_of_int:
	lw $v0, 0($sp)
	jr $ra


# intOfReal:int( num:real ) -> ($v0)
intOfReal:
	l.s $f28, 0($sp)
	cvt.w.s $f28, $f28
	mfc1 $v0, $f28
	jr $ra


# realOfInt:real( num:int ) -> ($f0)
realOfInt:
	lw $a0, 0($sp)
	mtc1 $a0, $f28
	cvt.s.w $f0, $f28
	jr $ra


# printReal:real( num:real ) -> ($f0)
printReal:
	li $v0, 2
	mov.s $f28, $f12	
	l.s $f12 0($sp)
	syscall
	mov.s $f12, $f28
	mov.s $f0, $f28
	jr $ra
	

# readReal:real( ) -> ($f0)
readReal:
	li $v0, 6
	syscall
	jr $ra

# end of standard library

