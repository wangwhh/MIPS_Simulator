ADDI $s1,$s2,50
ADDI $s0,$s1,77
ADD $s0,$s1,$s0
SUB $s0,$s0,$s1
AND $s0,$s0,$zero
BEQ $s0,$zero,L1