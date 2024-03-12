    add $s0, $imm1, $zero, $zero, 8, 0
    out $zero, $imm1, $zero, $imm2, 1, 1
    out $zero, $imm1, $zero, $imm2, 6, handle
read:
    beq $zero, $s0, $imm1, $imm2, 0, halting
    add $s1, $imm1, $zero, $zero, 1, 0
    add $t0, $s0, $imm1, $zero, -1, 0
    out $zero, $imm1, $zero, $t0, 15, 0
    out $zero, $imm1, $zero, $imm2, 16, 0
    out $zero, $imm1, $zero, $imm2, 14, 1
waitRead:
    beq $zero, $zero, $zero, $imm2, 0, waitRead
write:
    add $s1, $imm1, $zero, $zero, 2, 0
    add $t0, $s0, $imm1, $zero, 0, 0
    out $zero, $imm1, $zero, $t0, 15, 0
    out $zero, $imm1, $zero, $imm2, 16, 0
    out $zero, $imm1, $zero, $imm2, 14, 2
    add $s0, $imm1, $s0, $zero, -1, 0
waitWrite:
    beq $zero, $zero, $zero, $imm2, 0, waitWrite
handle:
    beq $zero, $s1, $imm1, $imm2, 1, handleWrite
    beq $zero, $s1, $imm1, $imm2, 2, handleRead
handleWrite:
    out $zero, $imm1, $zero, $imm2, 7, write
    reti $zero, $zero, $zero, $zero, 0, 0
handleRead:
    out $zero, $imm1, $zero, $imm2, 7, read
    reti $zero, $zero, $zero, $zero, 0, 0
halting:
    halt $zero, $zero, $zero, $zero, 0, 0