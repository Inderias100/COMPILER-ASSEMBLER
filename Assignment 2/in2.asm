lw $t0, 0($s0)
sw $t1, 4($s0)
slt $t2, $t0, $t1
add $t3, $t2, $zero
lw $s1, 8($sp)
sw $s1, 12($sp)
slt $s2, $s1, $t3