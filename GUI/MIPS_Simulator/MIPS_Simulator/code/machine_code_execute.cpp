#include "data_transform.h"
#include "machine_code_execute.h"
#include "invert_asm.h"
#include "assembler.h"

vector<string>registers(32);
vector<string>f_registers(32);
string register_name[32] = { "zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7",
"s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra" };
int PC;
int cur;
extern vector<string>memory;
extern string file_path;
extern vector<string>asm_codes;
extern map<string, int> Label;
extern int file_opened;

void exeInitialize()
{
    memory.clear();
    asm_codes.clear();
    Label.clear();
    if (file_opened == 1) {
        if (file_path.substr(file_path.length() - 4, 4) == ".asm") 
            MIPSassembler(file_path);
        else if (file_path.substr(file_path.length() - 4, 4) == ".bin")
            Disassemble(file_path);
    }
    for(int i = memory.size(); i < 65536; i++) memory.push_back("00000000");
    for(int i = 0; i<32; i++) registers[i] = "00000000000000000000000000000000";
    for (int i = 0; i < 32; i++) f_registers[i] = "00000000000000000000000000000000";
    registers[29] = "00000000000000000011111111111111";  //$sp = 0x3FFF
    PC = 0;
}

void Execute_instruction(string s)
{
    string op = s.substr(0,6);
    int rs, rt, rd;
    int ft, fs, fd;
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
        registers[31] = Int2Bin(PC, 32);
        string address = Int2Bin(PC, 32).substr(0,4) + s.substr(6,26) + "00";
        PC = StrToUnsign(address);
    }
    else if (op == "000010")//j
    {
        string address = Int2Bin(PC, 32).substr(0, 4) + s.substr(6, 26) + "00";
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
    //Here is the part of float&double number below.

    else if (op == "010001")
    {
        string fmt = s.substr(6, 5);
        ft = StrToUnsign(s.substr(11, 5));
        fs = StrToUnsign(s.substr(16, 5));
        fd = StrToUnsign(s.substr(21, 5));
        string funct = s.substr(26, 6);

        float t, s, d;
        t = Bin2Float(f_registers[ft]);
        s = Bin2Float(f_registers[fs]);
        //d = Bin2Float(f_registers[fd]);

        double tt, ss, dd;
        tt = Bin2Double(f_registers[ft], f_registers[ft + 1]);
        ss = Bin2Double(f_registers[fs], f_registers[fs + 1]);
        //dd = Bin2Double(f_registers[fd], f_registers[fd + 1]);

        if ((funct == "000000") && (fmt == "10000"))//add.s
        {

            d = t + s;
            f_registers[fd] = Float2Bin(d);
        }
        if ((funct == "000001") && (fmt == "10000"))//sub.s
        {
            d = t - s;
            f_registers[fd] = Float2Bin(d);
        }
        if ((funct == "000010") && (fmt == "10000"))//mul.s
        {
            d = t * s;
            f_registers[fd] = Float2Bin(d);
        }
        if ((funct == "000011") && (fmt == "10000"))//div.s
        {
            d = t / s;
            f_registers[fd] = Float2Bin(d);
        }
        if ((funct == "000000") && (fmt == "10001"))//add.d
        {         
            dd = ss + tt;
            f_registers[fd] = Double2Bin(dd).substr(0,32);
            f_registers[fd + 1] = Double2Bin(dd).substr(32);
        }
        if ((funct == "000001") && (fmt == "10001"))//sub.d
        {
            dd = tt - ss;
            f_registers[fd] = Double2Bin(dd).substr(0, 32);
            f_registers[fd + 1] = Double2Bin(dd).substr(32);
        }
        if ((funct == "000010") && (fmt == "10001"))//mul.d
        {
            dd = ss * tt;
            f_registers[fd] = Double2Bin(dd).substr(0, 32);
            f_registers[fd + 1] = Double2Bin(dd).substr(32);
        }
        if ((funct == "000011") && (fmt == "10001"))//div.d
        {
            dd = ss / tt;
            f_registers[fd] = Double2Bin(dd).substr(0, 32);
            f_registers[fd + 1] = Double2Bin(dd).substr(32);
        }
    }
    else if (op == "110001")//lwc1
    {
        int base = StrToUnsign(s.substr(6, 5));
        int fd = StrToUnsign(s.substr(11, 5));
        string immediate = s.substr(16, 16);
        int address = StrToUnsign(registers[base].substr(16, 16)) * 4 + StrToSign(immediate);
        f_registers[fd] = memory[address] + memory[address + 1] + memory[address + 2] + memory[address + 3];
    }
    else if (op == "111001")//swc1
    {
        int base = StrToUnsign(s.substr(6, 5));
        int fd = StrToUnsign(s.substr(11, 5));
        string immediate = s.substr(16, 16);
        int address = StrToUnsign(registers[base].substr(16, 16)) * 4 + StrToSign(immediate);
        memory[address] = f_registers[fd].substr(0, 8);
        memory[address + 1] = f_registers[fd].substr(8, 8);
        memory[address + 2] = f_registers[fd].substr(16, 8);
        memory[address + 3] = f_registers[fd].substr(24, 8);
    }
    else if (op == "110101")//ldc1
    {
        int base = StrToUnsign(s.substr(6, 5));
        int fd = StrToUnsign(s.substr(11, 5));
        string immediate = s.substr(16, 16);
        int address = StrToUnsign(registers[base].substr(16, 16)) * 4 + StrToSign(immediate);
        f_registers[fd] = memory[address] + memory[address + 1] + memory[address + 2] + memory[address + 3];
        f_registers[fd + 1] = memory[address + 4] + memory[address + 5] + memory[address + 6] + memory[address + 7];
    }
    else if (op == "111101")//sdc1
    {
        int base = StrToUnsign(s.substr(6, 5));
        int fd = StrToUnsign(s.substr(11, 5));
        string immediate = s.substr(16, 16);
        int address = StrToUnsign(registers[base].substr(16, 16)) * 4 + StrToSign(immediate);
        memory[address] = f_registers[fd].substr(0, 8);
        memory[address + 1] = f_registers[fd].substr(8, 8);
        memory[address + 2] = f_registers[fd].substr(16, 8);
        memory[address + 3] = f_registers[fd].substr(24, 8);
        memory[address + 4] = f_registers[fd + 1].substr(0, 8);
        memory[address + 5] = f_registers[fd + 1].substr(8, 8);
        memory[address + 6] = f_registers[fd + 1].substr(16, 8);
        memory[address + 7] = f_registers[fd + 1].substr(24, 8);
    }
}