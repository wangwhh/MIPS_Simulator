#include"assembler.h"

void GetLabel() {
	//读入所有的label
	ifstream fin;
	fin.open("code.txt", ios::in);
	if (!fin.is_open()) {
		cout << "读取文件失败" << endl;
		return;
	}
	while (fin.peek() != EOF) {
		getline(fin, buffer);
		if (buffer[0] != 0 && buffer[0] != '/') {
			string op;
			string binary;
			replace(buffer.begin(), buffer.end(), '(', ' ');
			replace(buffer.begin(), buffer.end(), ')', ' ');
			//cout << buffer << endl;
			replace(buffer.begin(), buffer.end(), '$', ' ');
			replace(buffer.begin(), buffer.end(), ',', ' ');
			replace(buffer.begin(), buffer.end(), '/', ' ');
			//cout << buffer << endl;
			transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
			stringstream strin(buffer);
			strin >> op;
			if (op[op.size() - 1] == ':') {
				Label[op.substr(0, op.size() - 1)] = addr;
				//cout << "in :" << op << endl;
			}
			else {
				addr += 1;
			}
		}
	}
	addr = 0;
	fin.close();
	return;
}

void OutputMachineCode() {
	cout << "2进制写法：" << endl;
	for (int i = 0; i < binarys.size(); i++) {
		cout << binarys[i] << endl;
	}
	cout << "16进制写法：" << endl;
	string hex;
	for (int i = 0; i < binarys.size(); i++) {
		hex = Bin2Hex(binarys[i]);
		cout << hex << endl;
	}

	ofstream fin_out;
	fin_out.open("machine_code.txt", ios::out);
	for (int i = 0; i < binarys.size(); i++) {
		fin_out << binarys[i] << endl;
	}
	fin_out.close();
	return;
}

void MIPSassembler() {
	GetLabel();
	ifstream fin_over;
	fin_over.open("code.txt", ios::in);
	while (fin_over.peek() != EOF) {
		getline(fin_over, buffer);
		//cout << endl;
		//cout << buffer << endl;
		if (buffer[0] != 0 && buffer[0] != '/') {
			string op;
			string binary;
			replace(buffer.begin(), buffer.end(), '(', ' ');
			replace(buffer.begin(), buffer.end(), ')', ' ');
			//cout << buffer << endl;
			replace(buffer.begin(), buffer.end(), '$', ' ');
			replace(buffer.begin(), buffer.end(), ',', ' ');
			replace(buffer.begin(), buffer.end(), '/', ' ');
			//cout << buffer << endl;
			transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
			stringstream strin(buffer);
			strin >> op;
			if (op[op.size() - 1] == ':') {
			}
			else {
				if (op[0] == 'j') {
					if (op[1] == 'a') {//jal
						string target;
						int t = 0;
						binary = "000011";
						strin >> target;
						if (Label.find(target) != Label.end()) {
							int addr = Label[target];
							binary += Int2Bin(addr, 26);
						}
						else if (target[0] == '0' && target[1] == 'x') {
							binary += Hex2Bin(target, 26);
						}
						else {
							for (int i = 0; i < target.size(); i++) {
								t *= 10;
								t += target[i] - '0';
							}
							binary += Int2Bin(t, 26);
						}
						binarys.push_back(binary);
					}
					else if (op[1] == 'r') {//jr
						string reg;
						strin >> reg;
						binary = "000000";
						binary += Reg2Bin(reg);
						binary += "000000000000000001000";
						binarys.push_back(binary);
					}
					else {//j
						binary = "000010";
						string target;
						int t = 0;
						strin >> target;
						if (Label.find(target) != Label.end()) {
							int addr = Label[target];
							//cout << "addr : " << addr << endl;
							binary += Int2Bin(addr, 26);
						}
						else if ((target[0] == '0' || target[0] == '0') && target[1] == 'x') {
							binary += Hex2Bin(target, 26);
						}
						else {
							for (int i = 0; i < target.size(); i++) {
								t *= 10;
								t += target[i] - '0';
							}
							binary += Int2Bin(t, 26);
						}
						binarys.push_back(binary);
					}
					//cout << binary << endl;
					//cout << BintoHex(binary) << endl;
				}
				else if (op == "add" || op == "sub" || op == "and" || op == "or" || op == "nor") {
					string rs, rt, rd;
					strin >> rd;
					strin >> rs;
					strin >> rt;
					binary = "000000";
					rd = Reg2Bin(rd);
					rs = Reg2Bin(rs);
					rt = Reg2Bin(rt);
					binary = binary + rs + rt + rd;
					if (op == "add") {
						binary += "00000100000";
					}
					else if (op == "sub") {
						binary += "00000100010";
					}
					else if (op == "and") {
						binary += "00000100100";
					}
					else if (op == "or") {
						binary += "00000100101";
					}
					else if (op == "nor") {
						binary += "00000100111";
					}
					binarys.push_back(binary);
					//cout << binary << endl;
					//cout << BintoHex(binary) << endl;
				}
				else if (op == "sll") {
					string rt, rd, shamt;
					int t = 0;
					strin >> rd;
					strin >> rt;
					strin >> shamt;
					binary = "00000000000";
					rd = Reg2Bin(rd);
					rt = Reg2Bin(rt);
					binary = binary + rt + rd;

					if (shamt[0] == '0' && shamt[1] == 'x') {
						binary += Hex2Bin(shamt, 5);
					}
					else {
						for (int i = 0; i < shamt.size(); i++) {
							t *= 10;
							t += shamt[i] - '0';
						}
						binary += Int2Bin(t, 5);
					}
					binary += "000000";
					binarys.push_back(binary);
					//cout << binary << endl;
					//cout << BintoHex(binary) << endl;
				}
				else if (op == "sw" || op == "lw") {
					string rs, rt, target;
					int t = 0;
					strin >> rt;
					strin >> target;
					strin >> rs;
					rs = Reg2Bin(rs);
					rt = Reg2Bin(rt);
					if (op == "lw") {
						binary = "100011";
					}
					else {
						binary = "101011";
					}
					binary += rs;
					binary += rt;
					if (target[0] == '0' && target[1] == 'x') {
						binary += Hex2Bin(target, 16);
					}
					else {
						for (int i = 0; i < target.size(); i++) {
							t *= 10;
							t += target[i] - '0';
						}
						binary += Int2Bin(t, 16);
					}
					binarys.push_back(binary);
					//cout << binary << endl;
					//cout << BintoHex(binary) << endl;
				}
				else {
					string rs, rt, target;
					int t = 0;
					strin >> rt;
					strin >> rs;
					strin >> target;
					rs = Reg2Bin(rs);
					rt = Reg2Bin(rt);
					if (op == "addi") {
						binary = "001000";
					}
					else if (op == "ori") {
						binary = "001101";
					}
					else if (op == "slti") {
						binary = "001010";
					}
					else if (op == "beq") {
						binary = "000100";
					}
					else if (op == "bne") {
						binary = "000101";
					}
					binary = binary + rs + rt;
					if (Label.find(target) != Label.end()) {
						int a = Label[target];
						a = a - addr - 1;
						binary += Int2Bin(a, 16);
					}
					else if (target[0] == '0' && target[1] == 'x') {
						binary += Hex2Bin(target, 16);
					}
					else {
						string str;
						if (target[0] == '-') {//负数
							for (int i = 1; i < target.size(); i++) {
								t *= 10;
								t += target[i] - '0';
							}
							//绝对值的二进制
							str = Int2Bin(t, 16);
							//取反
							for (int i = 0; i < str.size(); i++) {
								str[i] = str[i] == '0' ? '1' : '0';
							}
							//加一，转化为int加1在转化为binary
							str = Bin2Int(str);
							t = 0;
							for (int i = 0; i < str.size(); i++) {
								t *= 10;
								t += str[i] - '0';
							}
							t = t + 1;
							str = Int2Bin(t, 16);
						}
						else
						{
							for (int i = 0; i < target.size(); i++) {
								t *= 10;
								t += target[i] - '0';
							}
							str = Int2Bin(t, 16);
						}
						binary += str;
					}
					binarys.push_back(binary);
					//cout << binary << endl;
					//cout << BintoHex(binary) << endl;
				}
				addr += 1;
			}
		}
	}
	fin_over.close();
	OutputMachineCode();
	return;
}