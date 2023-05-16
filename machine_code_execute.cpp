#include "machine_code_execute.h"

int StrToSign(string s)
{
	int num;
	if(s[0]=='0') num = 0;
	else num = -1;
	for(int i=0;i<s.size();i++)
	{
		num = num<<1;
		if(s[i]=='1') num++;
	}
	return num;
}

int StrToUnsign(string s)
{
	int t = 0;
	for (int i = 0; i < s.size(); i++)
    {
		t *= 2;
		t += s[i] - '0';
	}
    return t;
}

void Initialize()
{
    for(int i=0;i<262140;i++) memory[i] = "00000000";
    for(int i=0;i<32;i++) registers[i] = "00000000000000000000000000000000";
    registers[29] = "1111111111111100";//$sp = 0xFFFC
    PC = 256;//0x0040
}

void Execute_instruction(string s)
{
    string op = s.substr(0,5);
    int rs, rt, rd;
    if(op == "000000")
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        rd = StrToUnsign(s.substr(16,20));
        int result;
        string funct = s.substr(26,31);
        if(funct == "001000")//jr
        {
            PC = StrToUnsign(registers[rs].substr(16,31)) * 4;
            return;
        }
        else if(funct == "100000")//add
            result = StrToSign(registers[rs]) + StrToSign(registers[rt]);
        else if(funct == "100010")//sub
            result = StrToSign(registers[rs]) - StrToSign(registers[rt]);
        else if(funct == "100100")//and
            result = StrToSign(registers[rs]) & StrToSign(registers[rt]);
        else if(funct == "100101")//or
            result = StrToSign(registers[rs]) | StrToSign(registers[rt]);
        else if(funct == "100111")//nor
            result = ~(StrToSign(registers[rs]) | StrToSign(registers[rt]));
        else if(funct == "000000")//sll
        {
            result = StrToSign(registers[rt]) << StrToUnsign(s.substr(21,25));
        }
        else if(funct == "000010")//srl
        {
            result = StrToSign(registers[rt]) >> StrToUnsign(s.substr(21,25));
        }
        registers[rd] = Int2Bin(result,32);
    }
    else if(op == "001000")//addi
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        int result = StrToSign(registers[rs]) + StrToSign(immediate);
        registers[rt] = Int2Bin(result,32);
    }
    else if(op == "001100")//andi
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        int result = StrToSign(registers[rs]) & StrToSign(immediate);
        registers[rt] = Int2Bin(result,32);
    }
    else if(op == "001101")//ori
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        int result = StrToSign(registers[rs]) | StrToSign(immediate);
        registers[rt] = Int2Bin(result,32);
    }
    else if(op == "100011")//lw
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        int address = StrToUnsign(registers[rs].substr(16,31)) * 4 + StrToSign(immediate);
        registers[rt] = memory[address] + memory[address+1] + memory[address+2] + memory[address+3];
    }
    else if(op == "101011")//sw
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        int address = StrToUnsign(registers[rs].substr(16,31)) * 4 + StrToSign(immediate);
        memory[address] = registers[rt].substr(0,7);
        memory[address+1] = registers[rt].substr(8,15);
        memory[address+2] = registers[rt].substr(16,23);
        memory[address+3] = registers[rt].substr(24,31);
    }
    else if(op == "000011")//jal
    {
        registers[31] = memory[PC] + memory[PC+1] + memory[PC+2] + memory[PC+3];
        string address = memory[PC].substr(0,5) + s.substr(6,31);
        PC = StrToUnsign(address) * 4;
    }
    else if(op == "000100")//beq
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        if(registers[rs]==registers[rt])
            PC += StrToSign(immediate) * 4;
    }
    else if(op == "000101")//bne
    {
        rs = StrToUnsign(s.substr(6,10));
        rt = StrToUnsign(s.substr(11,15));
        string immediate = s.substr(16,31);
        if(registers[rs]!=registers[rt])
            PC += StrToSign(immediate) * 4;
    }
}