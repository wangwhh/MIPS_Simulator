#include "machine_code_execute.h"



void Initialize()
{
    for(int i=0;i<262140;i++) memory[i] = "00000000";
    for(int i=0;i<32;i++) registers[i] = "00000000000000000000000000000000";
    registers[29] = "1111111111111100";//$sp = 0xFFFC
    PC = "0000000001000000";//0x0040
}

void Execute_instruction(string s)
{
    string op = s.substr(0,5);
    if(op == "000000")//add
    {

    }
    else if(op == "")//addi
    {

    }
}