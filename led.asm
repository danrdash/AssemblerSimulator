	add $sp, $zero, $imm, 1500 #set $sp to random place to start stack
	add $sp, $sp, $imm, -4 #make place for 6 variables
	sw $s0, $sp, $imm, 0		# save $s0
	sw $s1, $sp, $imm, 1		# save $s1
	sw $s2, $sp, $imm, 2		# save $s2
	sw $a0, $sp, $imm, 3		#save $a0
	add $s0, $zero , $imm, 1		#init $s0 =1 = pull_reg
	add $s1, $zero, $zero		#init $s1 = click
	add $s2, $zero, $zero		#init $s2 = counter 
	add $a0, $zero, $imm, 1		#init $a0 = 1
	add $a1, $zero, $imm, 1		#init $a1= 1 =mask
	led $s0, $a0, $a0			#write $s0 = 1 into IOReg[1]
while:
	add $s2, $s2, $imm, 100000		#counter++
	beq $imm, $zero, $zero, condmainif1		#go to condmainif
mainif1:
	led $s0, $a0, $zero		#read IOReg[1] into $s0
	beq $imm, $zero, $zero, condsubif1		#check cond for subif1
subif1:
	add $s0, $zero, $imm, 1		#$s0 = pull reg equals 1
	led $s0, $a0, $a0			#write ldreg = $s0
	add $s2, $zero, $zero		#init counter = 0
	beq $imm, $zero, $zero, while	#go back to while
condsubif1:
	add $t0, $zero, $imm, 127	
	bgt $imm, $s0, $t0,	subif1		#if $s0 > $t0 | if ldreg >=128 enter cond	
	led $s0, $a0, $zero		#read IOReg[1] into $s0
	add $t0, $zero $imm, 1		#init $t0 = 1
	sll $s0, $s0, $t0			#shift $s0 left by $t0
	led $s0, $a0, $a0			#write	$s0 into ldreg
	add $s2, $zero, $zero		#init counter = 0, finish mainif1
	beq $imm, $zero, $zero, condmainifA	 #check cond for mainif2
condmainif1:
	add $t2, $zero, $imm, 100000		#init $t2 to 1
	bne $imm, $t2, $s2, mainif2		#go to mainif2 if counter does not equal to 1
	beq $imm, $t2, $s2, condmainif2 #if counter = 1 cond 2 is complete
condmainif2:
	add $t2, $zero, $imm, 2			#init $t2 = 2
	led $s1, $t2, $zero				#read from IOReg[2] into $s1
	and $t1, $a1, $s1				# $t1 = click & mask
	bne $imm, $t1, $zero, mainif1	# !click & mask is true BRANCH out, if cond fails we go to condmainifA
	beq $imm, $zero, $zero, condmainifA 
mainif2:
	led $s0, $a0, $zero		#read IOReg[1] into $s0
	beq $imm, $zero, $zero, condsubifA		#check cond for subifA
subifA:
	add $s0, $zero, $imm, 128			#init $s0 = pullreg to 128
	led $s0, $a0, $a0				#write ldreg = $s0 = pull_reg
	add $s2, $zero, $zero			#init counter = 0
	beq $imm, $zero, $zero, while	#continue to while
condsubifA:
	beq $imm, $s0, $zero, subifA		#go to subifA if ldreg = 0
	led $s0, $a0, $zero		#read IOReg[1] into $s0
	beq $imm, $zero, $zero, condsubifB		#check cond for subifB
subifB:
	add $s0, $zero, $imm, 128			#init $s0 = pullreg to 128
	led $s0, $a0, $a0				#write ldreg = $s0 
	add $s2, $zero, $zero			#init counter = 0
	beq $imm, $zero, $zero, end		#go to end
condsubifB:
	add $t0, $zero, $imm, 1					#t0 =1
	ble $imm, $s0, $t0, subifB				#if $s0<2 go to subifB
	add $t0, $zero $imm, 1		#init $t0 = 1
	sra $s0, $s0, $t0			#shift right arithmetic $s0 >> $t0
	led $s0, $a0, $a0			#write	$s0 into ldreg
	add $s2, $zero, $zero		#init counter = 0, finish mainif2
	beq $imm, $zero, $zero, end	#go to end
condmainifA:
	add $t2, $zero, $imm, 100000		#init $t2 to 1
	bne $imm, $t2, $s2, end		#go to end if cond fails {{{{TO DOOO}}}}
	beq $imm, $t2, $s2, condmainifB #if counter = 1 cond 2 is complete
condmainifB:
	add $t2, $zero, $imm, 2			#init $t2 = 2
	led $s1, $t2, $zero				#read from IOReg[2] into $s1
	and $t1, $a1, $s2				#t1 = click & mask
	bne $imm, $t1, $zero, mainif2	#click & mask is true
end:
	beq $imm, $zero, $zero, while	# while(1)
