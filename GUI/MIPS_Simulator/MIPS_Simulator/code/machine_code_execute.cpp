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
extern int file_opened;

float Bin2Float(string s)
{
    int sign = (s[0] == '1') ? -1 : 1;

    string exponentString = s.substr(1, 8);
    int exponent = StrToUnsign(exponentString) - 127;

    string mantissaString = s.substr(9);
    float mantissa = 1.0f;

    for (int i = 0; i < mantissaString.length(); ++i)
        if (mantissaString[i] == '1')
        {
            mantissa += 1.0f / (1 << (i + 1));
        }

    //cout<<exponent<<endl;
    return sign * mantissa * pow(2, exponent);
}

string Float2Bin(float value)
{

    string signBit = (value < 0) ? "1" : "0";

    float absValue;
    if (value >= 0)
        absValue = value;
    else
        absValue = -1 * value;

    int integerPart = static_cast<int>(absValue);
    float fractionalPart = absValue - integerPart;

    string integerString = "";

    for (int i = 0; integerPart > 0; i++)//mode r
    {
        if (integerPart % 2 == 1)
            integerString = "1" + integerString;
        else
            integerString = "0" + integerString;
        integerPart /= 2;
    }

    // 将小数部分转换为二进制字符串
    string fractionalString = "";

    for (int i = 0; fractionalPart > 0 && i < 23; i++)//multi 2
    {
        if (fractionalPart * 2 >= 1) fractionalString += "1";
        else fractionalString += "0";
        fractionalPart *= 2;
        fractionalPart -= int(fractionalPart);
    }

    // 计算浮点数的指数部分
    int exponent = 0;

    if (static_cast<int>(absValue) != 0)//left move
    {
        exponent = integerString.length() - 1;
        for (int i = 0; i < exponent; i++)
            fractionalString = integerString[integerString.length() - 1 - i] + fractionalString;
    }
    else if (fractionalPart != 0)//right move
    {
        // 寻找小数部分的首位1位置
        size_t firstOnePos = fractionalString.find_first_of("1");
        exponent = -static_cast<int>(firstOnePos) - 1;//index
        for (int i = 0; i < -1 * exponent; i++)
            fractionalString = fractionalString.substr(1);
    }

    exponent += 127;

    // 补齐小数部分的零位
    while (fractionalString.length() < 23) {
        fractionalString += "0";
    }

    // 将指数部分转换为二进制字符串
    string exponentString = Int2Bin(exponent, 8);

    // 拼接符号位、指数部分和尾数部分
    return (signBit + exponentString + fractionalString).substr(0, 32);
}

double Bin2Double(string s1, string s2)
{
    string s = s1 + s2;

    int sign = (s[0] == '1') ? -1 : 1;

    string exponentString = s.substr(1, 11);
    int exponent = StrToUnsign(exponentString) - 1023;

    string mantissaString = s.substr(12);
    double mantissa = 1.0;

    for (int i = 0; i < mantissaString.length(); ++i)
        if (mantissaString[i] == '1')
        {
            mantissa += 1.0 / (1 << (i + 1));
        }

    //cout<<exponent<<endl;
    return sign * mantissa * pow(2, exponent);
}

string Double2Bin(double value)//可以通过Double2Bin(3.1415926...).str(0,32)和Double2Bin(3.1415926...).str(32)来调用
{

    string signBit = (value < 0) ? "1" : "0";

    double absValue;
    if (value >= 0)
        absValue = value;
    else
        absValue = -1 * value;

    int integerPart = static_cast<int>(absValue);
    double fractionalPart = absValue - integerPart;

    string integerString = "";

    for (int i = 0; integerPart > 0; i++)//mode r
    {
        if (integerPart % 2 == 1)
            integerString = "1" + integerString;
        else
            integerString = "0" + integerString;
        integerPart /= 2;
    }

    // 将小数部分转换为二进制字符串
    string fractionalString = "";

    for (int i = 0; fractionalPart > 0 && i < 52; i++)//multi 2
    {
        if (fractionalPart * 2 >= 1) fractionalString += "1";
        else fractionalString += "0";
        fractionalPart *= 2;
        fractionalPart -= int(fractionalPart);
    }

    // 计算浮点数的指数部分
    int exponent = 0;

    if (static_cast<int>(absValue) != 0)//left move
    {
        exponent = integerString.length() - 1;
        for (int i = 0; i < exponent; i++)
            fractionalString = integerString[integerString.length() - 1 - i] + fractionalString;
    }
    else if (fractionalPart != 0)//right move
    {
        // 寻找小数部分的首位1位置
        size_t firstOnePos = fractionalString.find_first_of("1");
        exponent = -static_cast<int>(firstOnePos) - 1;//index
        for (int i = 0; i < -1 * exponent; i++)
            fractionalString = fractionalString.substr(1);
    }

    exponent += 1023;

    // 补齐小数部分的零位
    while (fractionalString.length() < 52) {
        fractionalString += "0";
    }

    // 将指数部分转换为二进制字符串
    string exponentString = Int2Bin(exponent, 11);

    // 拼接符号位、指数部分和尾数部分
    return (signBit + exponentString + fractionalString).substr(0, 64);
}

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
    for (int i = 0; i < 32; i++) f_registers[i] = "00000000000000000000000000000000";
    registers[29] = "00000000000000001111111111111100";  //$sp = 0xFFFC
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
    //Here is the part of float&double number below.

    else if (op == "010001")
    {
        string fmt = s.substr(6, 5);
        ft = StrToUnsign(s.substr(11, 5));
        fs = StrToUnsign(s.substr(16, 5));
        fd = StrToUnsign(s.substr(21, 5));
        string funct = s.substr(26, 6);
        int result;

        float t, s, d;
        //t = Bin2Float(f_registers[ft]);
        s = Bin2Float(f_registers[fs]);
        d = Bin2Float(f_registers[fd]);

        double tt, ss, dd;
        //tt = Bin2Float(f_registers[ft]);
        ss = Bin2Double(f_registers[fs], f_registers[fs + 1]);
        dd = Bin2Double(f_registers[fd], f_registers[fd + 1]);

        if ((funct == "000000") && (fmt == "10000"))//add.s
        {
            t = s + d;
            f_registers[ft] = Float2Bin(t);
        }
        if ((funct == "000001") && (fmt == "10000"))//sub.s
        {
            t = s - d;
            f_registers[ft] = Float2Bin(t);
        }
        if ((funct == "000010") && (fmt == "10000"))//mul.s
        {
            t = s * d;
            f_registers[ft] = Float2Bin(t);
        }
        if ((funct == "000011") && (fmt == "10000"))//div.s
        {
            t = s / d;
            f_registers[ft] = Float2Bin(t);
        }
        if ((funct == "000000") && (fmt == "10001"))//add.d
        {
            tt = ss + dd;
            f_registers[ft] = Double2Bin(tt).substr(0,32);
            f_registers[ft + 1] = Double2Bin(tt).substr(32);
        }
        if ((funct == "000001") && (fmt == "10001"))//sub.d
        {
            tt = ss - dd;
            f_registers[ft] = Double2Bin(tt).substr(0, 32);
            f_registers[ft + 1] = Double2Bin(tt).substr(32);
        }
        if ((funct == "000010") && (fmt == "10001"))//mul.d
        {
            tt = ss * dd;
            f_registers[ft] = Double2Bin(tt).substr(0, 32);
            f_registers[ft + 1] = Double2Bin(tt).substr(32);
        }
        if ((funct == "000011") && (fmt == "10001"))//div.d
        {
            tt = ss / dd;
            f_registers[ft] = Double2Bin(tt).substr(0, 32);
            f_registers[ft + 1] = Double2Bin(tt).substr(32);
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