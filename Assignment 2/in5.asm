lw $t0, 0($a0)
lw $t1, 4($a0)
add $t2, $t0, $t1
slt $t3, $t2, $t1
andi $t4, $t3, 1
beq $t4, $zero, finish
sw $t2, 8($a0)
finish:
or $v0, $t2, $zero