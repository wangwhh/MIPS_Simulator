addi $t2,$t1,1
sll $t2,$t2,30
addi $t0,$t1,1024
sw $t2,0($t0)
lwc1 $f1,0($t0)
addi $t3,$t1,16457
sll $t3,$t3,4
addi $t3,$t3,1
sll $t3,$t3,12
sw $t3,0($t0)
lwc1 $f3,0($t0)
add.s $f5 ,$f1, $f3
sub.s $f5 ,$f1, $f3
mul.s $f5 ,$f1, $f3
div.s $f5 ,$f1, $f3
swc1 $f5,0($t0)