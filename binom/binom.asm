    sll $sp, $imm1, $imm2, $zero, 1, 11
    lw $a0, $imm1, $zero, $zero, 256, 0
    lw $a1, $imm1, $zero, $zero, 257, 0
    add $ra, $imm2, $zero, $zero, 0, close
binom:
    add $sp, $sp, $imm1, $zero, -4, 0
    sw $zero, $sp, $imm1, $s0, 0, 0
    sw $zero, $sp, $imm1, $s1, 1, 0
    sw $zero, $sp, $imm1, $s2, 2, 0
    sw $zero, $sp, $imm1, $ra, 3, 0
    add $s0, $a0, $zero, $zero, 0, 0
    add $s1, $a1, $zero, $zero, 0, 0
    add $s2, $zero, $zero, $zero, 0, 0
    beq $zero, $s1, $imm1, $imm2, 0, ret1 
    beq $zero, $s1, $s0, $imm2, 0, ret1 
    add $a0, $s0, $imm1, $zero, -1, 0
    add $a1, $s1, $imm1, $zero, -1, 0
    jal $ra, $zero, $zero, $imm2, 0, binom
    add $s2, $s2, $v0, $zero, 0, 0
    add $a0, $s0, $imm1, $zero, -1, 0
    add $a1, $s1, $imm1, $zero, 0, 0
    jal $ra, $zero, $zero, $imm2, 0, binom
    add $s2, $s2, $v0, $zero, 0, 0
    add $v0, $s2, $imm1, $zero, 0, 0
    beq $zero, $zero, $zero, $imm2, 0, doneBinom
ret1:
    add $v0, $zero, $imm1, $zero, 1, 0
    beq $zero, $zero, $zero, $imm2, 0, doneBinom
doneBinom:
    lw $s0, $sp, $imm1, $zero, 0, 0
    lw $s1, $sp, $imm1, $zero, 1, 0
    lw $s2, $sp, $imm1, $zero, 2, 0
    lw $ra, $sp, $imm1, $zero, 3, 0
    add $sp, $sp, $imm1, $zero, 4, 0
    beq $zero, $zero, $zero, $ra, 0, 0
close:
    sw $zero, $zero, $imm1, $v0, 258, 0
    halt $zero, $zero, $zero, $zero, 0, 0