        addi $s0, $zero, 1
        addi $s1, $zero, 2
        addi $s2, $zero, 3
        addi $s3, $zero, 4
        addi $s4, $zero, 5
        addi $t7, $zero, 0
        addi $t8, $zero, 1024
        sw $s0, 0($t8)
        sw $s1, 4($t8)
        sw $s2, 8($t8)
        sw $s3, 12($t8)
        sw $s4, 16($t8)
        add $t0, $zero, $zero
        add $t1, $zero, $zero
loop:   add $t7, $t0, $t8
        lw $t2, 0($t7)
        add $t1, $t1, $t2
        andi $t1, $t1, 255
        or $t1, $t1, $t2
        ori $t1, $t1, 256
        sub $t1, $t1, $t2
        nor $t1, $t1, $t2
        and $t1, $t1, $t2
        sll $t3, $t1, 2
        srl $t3, $t1, 1
        sw $t1, 0($t7)
        bne $t0, $s3, exit
        beq $t0, $s2, function
        addi $t0, $t0, 1
        j loop
exit:   jal function
        addi $t4, $zero, 1
        sub $t4, $t4, $t5
        sll $t4, $t4, 10
        jr $ra
function:   addi $t5, $zero, 1
            sub $t5, $t5, $t4
            sll $t5, $t5, 10
            jr $ra