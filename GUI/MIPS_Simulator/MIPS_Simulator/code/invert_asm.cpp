#include "assembler.h"
#include "data_transform.h"
#include "invert_asm.h"

extern vector<string> asm_codes;
extern vector<string> memory;
extern string file_path;

void Disassemble(string file_path)
{
	ifstream fin;
	string buffer;
	fin.open(file_path, ios::in);
	if (!fin.is_open()) {
		cout << "¶ÁÈ¡ÎÄ¼þÊ§°Ü" << endl;
		return;
	}
	while (fin.peek() != EOF) {
		getline(fin, buffer);
		memory.push_back(buffer.substr(0, 8));
		memory.push_back(buffer.substr(8, 8));
		memory.push_back(buffer.substr(16, 8));
		memory.push_back(buffer.substr(24, 8));
		asm_codes.push_back(invert_line(buffer));
	}
	fin.close();
}

string invert_line(string buffer)
{
	string op = buffer.substr(0, 6);
	string rs, rt, rd, shamt, func, immediate, addr;
	string ret;
	int flag = 0;
	if (op == "000000") {
		func = buffer.substr(26, 6);
		if (func == "100000") {
			ret = "add";
			flag = 1;
		}
		else if (func == "100010") {
			ret = "sub";
			flag = 1;
		}
		else if (func == "100100") {
			ret = "and";
			flag = 1;
		}
		else if (func == "100101") {
			ret = "or";
			flag = 1;
		}
		else if (func == "101010") {
			ret = "slt";
			flag = 1;
		}
		else if (func == "000000") {
			ret = "sll";
			flag = 2;
		}
		else if (func == "000010") {
			ret = "srl";
			flag = 2;
		}
		else if (func == "001000") {
			ret = "jr";
			flag = 3;
		}
		if (flag == 1) {
			rs = buffer.substr(6, 5);
			rt = buffer.substr(11, 5);
			rd = buffer.substr(16, 5);
			ret = ret + " " + get_register(rs)+", "+get_register(rt) + ", " + get_register(rd);
		}
		else if (flag == 2) {
			rt = buffer.substr(11, 5);
			rd = buffer.substr(16, 5);
			shamt = buffer.substr(21, 5);
			ret = ret + " " + get_register(rt) + ", " + get_register(rd) + ", " + Bin2Int(shamt);
		}
		else if (flag == 3) {
			rs = buffer.substr(6, 5);
			ret = ret + " " + get_register(rs);
		}
	}
	else{
		if (op == "001000") {
			ret = "addi";
			flag = 1;
		}
		else if (op == "001100") {
			ret = "andi";
			flag = 1;
		}
		else if (op == "001101") {
			ret = "ori";
			flag = 1;
		}
		else if (op == "100011") {
			ret = "lw";
			flag = 2;
		}
		else if (op == "101011") {
			ret = "sw";
			flag = 2;
		}
		else if (op == "000100") {
			ret = "beq";
			flag = 1;
		}
		else if (op == "000101") {
			ret = "bne";
			flag = 1;
		}
		else if (op == "000010") {
			ret = "j";
			flag = 3;
		}
		else if (op == "000011") {
			ret = "jal";
			flag = 3;
		}
		if (flag == 1) {
			rs = buffer.substr(6, 5);
			rt = buffer.substr(11, 5);
			immediate = buffer.substr(16, 16);
			ret = ret + " " + get_register(rs) + ", " + get_register(rt) + ", " + Bin2Int(immediate);
		}
		else if (flag == 2) {
			rs = buffer.substr(6, 5);
			rt = buffer.substr(11, 5);
			immediate = buffer.substr(16, 16);
			ret = ret + " " + get_register(rs) + ", " + Bin2Int(immediate) + "(" + get_register(rt) + ")";
		}
		else if (flag == 3) {
			addr = buffer.substr(6, 26);
			ret = ret + " " + Bin2Int(addr);
		}
	}
	if (flag != 0) return ret;
	else return NULL;
}

string get_register(string reg)
{
	int n = stoi(reg, nullptr, 2);
	string register_name[32] = {"zero","at","v0","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5",
		"t6","t7","s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra"};
	string ret = "$" + register_name[n];
	return ret;
}
