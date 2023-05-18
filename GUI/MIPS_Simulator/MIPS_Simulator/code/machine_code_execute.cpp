#include "data_transform.h"
#include "machine_code_execute.h"
#include "invert_asm.h"
#include "assembler.h"

vector<string>registers(32);
string register_name[32] = { "zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7",
"s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra" };
int PC;
int cur;
extern vector<string>memory;
extern string file_path;
extern vector<string>asm_codes;
extern int file_opened;

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

void exeInitialize()
{
    memory.clear();
    asm_codes.clear();
    if (file_opened == 1) {
        if (file_path.substr(file_path.length() - 4, 4) == ".asm") 
            MIPSassembler(file_path);
        else if (file_path.substr(file_path.length() - 4, 4) == ".bin")
            Disassemble(file_path);
    }
    for(int i = memory.size(); i < 65536; i++) memory.push_back("00000000");
    for(int i = 0; i<32; i++) registers[i] = "00000000000000000000000000000000";
    registers[29] = "00000000000000001111111111111100";  //$sp = 0xFFFC
    PC = 0;
}

void Execute_instruction(string s)
{
    string op = s.substr(0,6);
    int rs, rt, rd;
    if(op == "000000")
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        rd = StrToUnsign(s.substr(16,5));
        int result;
        string funct = s.substr(26,6);
        if(funct == "001000")//jr
        {
            PC = StrToUnsign(registers[rs].substr(16,16));
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
            result = StrToSign(registers[rt]) << StrToUnsign(s.substr(21,5));
        }
        else if(funct == "000010")//srl
        {
            result = StrToSign(registers[rt]) >> StrToUnsign(s.substr(21,5));
        }
        registers[rd] = Int2Bin(result,32);
    }
    else if(op == "001000")//addi
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        int result = StrToSign(registers[rs]) + StrToSign(immediate);
        registers[rt] = Int2Bin(result,32);
    }
    else if(op == "001100")//andi
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        int result = StrToSign(registers[rs]) & StrToSign(immediate);
        registers[rt] = Int2Bin(result,32);
    }
    else if(op == "001101")//ori
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        int result = StrToSign(registers[rs]) | StrToSign(immediate);
        registers[rt] = Int2Bin(result,32);
    }
    else if(op == "100011")//lw
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        int address = StrToUnsign(registers[rs].substr(16,16)) * 4 + StrToSign(immediate);
        registers[rt] = memory[address] + memory[address+1] + memory[address+2] + memory[address+3];
    }
    else if(op == "101011")//sw
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        int address = StrToUnsign(registers[rs].substr(16,16)) * 4 + StrToSign(immediate);
        memory[address] = registers[rt].substr(0,8);
        memory[address+1] = registers[rt].substr(8,8);
        memory[address+2] = registers[rt].substr(16,8);
        memory[address+3] = registers[rt].substr(24,8);
    }
    else if(op == "000011")//jal
    {
        registers[31] = memory[PC] + memory[PC+1] + memory[PC+2] + memory[PC+3];
        string address = memory[PC].substr(0,4) + s.substr(6,26) + "00";
        PC = StrToUnsign(address);
    }
    else if(op == "000100")//beq
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        if(registers[rs]==registers[rt])
            PC += StrToSign(immediate) * 4;
    }
    else if(op == "000101")//bne
    {
        rs = StrToUnsign(s.substr(6,5));
        rt = StrToUnsign(s.substr(11,5));
        string immediate = s.substr(16,16);
        if(registers[rs]!=registers[rt])
            PC += StrToSign(immediate) * 4;
    }
}