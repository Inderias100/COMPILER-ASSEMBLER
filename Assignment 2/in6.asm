addi $8, $0, 5
addi $9, $0, 10
repeat: and $10, $8, $9
ori $11, $10, 7
slt $12, $11, $9
beq $12, $0, repeat
j exit
exit:
sw $11, 0($29)