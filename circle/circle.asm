    sll $sp, $imm1, $imm2, $zero, 1, 11
    lw $s0, $imm1, $zero, $zero, 256, 0
    add $s1, $zero, $zero, $zero, 0, 0
for1:
    beq $zero, $s1, $imm1, $imm2, 256, exit1
    add $s2, $zero, $zero, $zero, 0, 0
for2:
    beq $zero, $s2, $imm1, $imm2, 256, exit2
    sub $t0, $s1, $imm1, $zero, 127, 0
    sub $t1, $s2, $imm1, $zero, 127, 0
    mac $t0, $t0, $t0, $zero, 0, 0
    mac $t1, $t1, $t1, $zero, 0, 0
    add $t0, $t0, $t1, $zero, 0, 0
    mac $t1, $s0, $s0, $zero, 0, 0
    bgt $zero, $t0, $t1, $imm2, 0, continue
    add $a0, $s1, $zero, $zero, 0, 0
    add $a1, $s2, $zero, $zero, 0, 0
    jal $ra, $zero, $zero, $imm2, 0, put
continue:
    add $s2, $s2, $imm1, $zero, 1, 0
    beq $zero, $zero, $zero, $imm2, 0, for2
exit2:
    add $s1, $s1, $imm1, $zero, 1, 0
    beq $zero, $zero, $zero, $imm2, 0, for1
exit1:
    halt $zero, $zero, $zero, $zero, 0, 0
put:
    sll $t0, $a0, $imm1, $zero, 8, 0
    add $t1, $t0, $a1, $zero, 0, 0
    out $zero, $imm1, $imm2, $t1, 20, 0
    out $zero, $imm1, $zero, $imm2, 21, 255
    out $zero, $imm1, $zero, $imm2, 22, 1
    beq $zero, $zero, $zero, $ra, 0, 0
.word 0x100 5