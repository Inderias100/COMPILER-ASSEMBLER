andi $t0, $s0, 1
ori $t1, $s1, 2
xor $t2, $t0, $t1
slt $t3, $t2, $zero
beq $t3, $zero, skip
j target
skip:
add $t4, $t4, $t4
target:
sw $t4, 0($sp)