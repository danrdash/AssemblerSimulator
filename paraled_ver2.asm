	add $sp, $zero, $imm, 1500 #set $sp to random place to start stack
	add $sp, $sp, $imm, -4 #make place for 4 variables
	sw $s0, $sp, $imm, 0		# save $s0
	sw $s1, $sp, $imm, 1		# save $s1
	sw $s2, $sp, $imm, 2		# save $s2
	sw $a0, $sp, $imm, 3		#save $a0
	add $t3, $zero, $imm, -1		#init $t3 =0 = hitwall 
	add $s0, $zero, $imm, 1		#init $s0 =1 = pull_reg
	add $s1, $zero, $zero		#init $s1 = click
	add $s2, $zero, $zero		#init $s2 = counter 
	add $a0, $zero, $imm, 1		#init $a0 = 1
	add $a1, $zero, $imm, 1		#init $a1= 1 =mask
	led $s0, $a0, $a0			#write $s0 = 1 into IOReg[1]
while:
	add $s2, $s2, $imm, 100000		#counter++
	add $t0, $zero $imm, 2		#set $t0=2
	led $s1, $t0, $zero			#read from IOReg[2] into $s1 AKA number of clicks
BOUNDSUP:
	add $t0, $zero, $imm, 127	# $t0 = 127
	led $s0, $a0, $zero			#read IOReg[1] into $s0
	bgt $imm, $s0, $t0, UPPER	#jump to upper if $s0 > $t0
BOUNDSLOW:
	add $t0, $zero, $imm, 1		#set $t0 to be 2
	led $s0, $a0, $zero			#read IOReg[1] into $s0
	ble $imm, $s0, $t0, LOWER	#jump to lower if $s0 <= 1
	beq $imm, $zero, $zero, condEE #didn't trigger any conditions, go back to while
UPPER:
	add $s0, $zero, $imm, 128	#set $s0 to 128
	led $s0, $a0, $a0			#write $s0 to IOReg[1]
	add $t3, $t3, $imm, 1		#hitwall++
	beq $imm, $zero, $zero, BOUNDSLOW					#check lower boundary aswell
LOWER:
	add $s0, $zero, $imm, 1		#init $s0 to 1
	led $s0, $a0, $a0			#write $s0 to IOReg[1]
	add $t3, $t3, $imm, 1		#hitwall++
condEE:
	and $t1, $a1, $s1			# $t1 = click &mask
	and $t2, $t3, $a1			# $t2 = hitwall & mask
	add $t2, $t1, $t2			# sum both results
	beq $imm, $zero, $t2, LEFT	# if clicks and hitwall are both even then their sum is 0 and we can enter condition and go left
condOO:
	add $t0, $zero, $imm, 2		#set $t0=2
	and $t1, $a1, $s1			# $t1 = click &mask
	and $t2, $t3, $a1			# $t2 = hitwall & mask
	add $t2, $t1, $t2			# sum both results
	beq $imm, $t0, $t2, LEFT	# if clicks and hitwall are both odd then their sum is 2 and we can enter condition and go left
condOE:
	add $t0, $zero, $imm, 1
	and $t1, $a1, $s1			# $t1 = click &mask
	and $t2, $t3, $a1			# $t2 = hitwall & mask
	add $t2, $t1, $t2			# sum both results
	beq $imm, $t0, $t2, RIGHT	# if clicks is odd and hitwall is even or vice versa then their sum is 1 and we can enter condition and go RIGHT
LEFT:
	led $s0, $a0, $zero		#read IOReg[1] into $s0
	add $t0, $zero $imm, 1		#init $t0 = 1
	sll $s0, $s0, $t0			#shift $s0 left by $t0
	led $s0, $a0, $a0			#write	$s0 into ldreg
	add $s2, $zero, $zero		#init counter = 0
	beq $imm, $zero, $zero, while #check bounds
RIGHT:
	led $s0, $a0, $zero		#read IOReg[1] into $s0
	add $t0, $zero $imm, 1		#init $t0 = 1
	sra $s0, $s0, $t0			#shift right arithmetic $s0 >> $t0
	led $s0, $a0, $a0			#write	$s0 into ldreg
	add $s2, $zero, $zero		#init counter = 0;
	beq $imm, $zero, $zero, while #check bounds
