# MIPS_Simulator
* 汇编器

data_tranform.cpp是一些进制转换和字符转换的工具函数

assembler.cpp是汇编器主体

接口为“assembler.h”头文件和void MIPSassembler()函数

函数引用后会将code.txt中的代码译为机器码，以二进制和十六进制输出到命令行，并以二进制输入到machine_code.txt中

调用汇编器时，将代码读入code.txt，然后使用MIPSassembler()，再从machine_code.txt读出即可
