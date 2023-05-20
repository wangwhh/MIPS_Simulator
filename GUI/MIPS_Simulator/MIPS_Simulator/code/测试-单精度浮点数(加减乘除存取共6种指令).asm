addi $t2,$t1,1
sll $t2,$t2,30
addi $t0,$t1,1024
sw $t2,0($t0)
lwc1 $f0,0($t0)
addi $t3,$t1,16457
sll $t3,$t3,4
addi $t3,$t3,1
sll $t3,$t3,12
sw $t3,0($t0)
lwc1 $f2,0($t0)
add.s $f4 ,$f0, $f2
sub.s $f4 ,$f0, $f2
mul.s $f4 ,$f0, $f2
div.s $f4 ,$f0, $f2
swc1 $f4,0($t0)