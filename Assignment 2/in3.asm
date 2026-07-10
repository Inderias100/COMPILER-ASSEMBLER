start:
addi $t0, $zero, 0
addi $t1, $zero, 10
loop:
beq $t0, $t1, end
add $t0, $t0, 1
j loop
end:
sw $t0, 0($s0)