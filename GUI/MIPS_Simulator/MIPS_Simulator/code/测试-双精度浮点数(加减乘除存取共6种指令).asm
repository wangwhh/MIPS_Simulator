add $t1,$zero,$zero
addi $t0,$t1,1024
addi $t2,$t1,1
sll $t2,$t2,25
addi $t2,$t2,18703
sll $t2,$t2,5
addi $t2,$t2,27
sw $t2,0($t0)
addi $t3,$t1,19730
sll $t3,$t3,15
addi $t3,$t3,27685
sll $t3,$t3,1
sw $t3,4($t0)
ldc1 $f0,0($t0)
addi $t4,$t1,3
sll $t4,$t4,30
sw $t4,8($t0)
ldc1 $f2,8($t0)
add.d $f4 ,$f0, $f2
sub.d $f4 ,$f0, $f2
mul.d $f4 ,$f0, $f2
div.d $f4 ,$f0, $f2
sdc1 $f4,0($t0)