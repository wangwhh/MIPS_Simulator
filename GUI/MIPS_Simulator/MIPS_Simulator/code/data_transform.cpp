#include"data_transform.h"

string Int2Bin(int val,int k) {
	string str = "";
	for (int i = k - 1; i >= 0; i--) {
		if (val & (1 << i))//&°´Î»¼ÆËã
			str += "1";
		else
			str += "0";
	}
	return str;
}

string Reg2Bin(string reg) {
	string str;
	switch (reg[0]) {
	case 'z':str = "00000"; break;
	case 'v': {if (reg[1] == '0') str = "00010";
			else str = "00011"; } break;
	case 'a': {
		switch (reg[1]) {
		case 't':str = "00001"; break;
		case '0':str = "00100"; break;
		case '1':str = "00101"; break;
		case '2':str = "00110"; break;
		case '3':str = "00111"; break;
		}
	}break;
	case 't': {
		switch (reg[1]) {
		case '0':str = "01000"; break;
		case '1':str = "01001"; break;
		case '2':str = "01010"; break;
		case '3':str = "01011"; break;
		case '4':str = "01100"; break;
		case '5':str = "01101"; break;
		case '6':str = "01110"; break;
		case '7':str = "01111"; break;
		case '8':str = "11000"; break;
		case '9':str = "11001"; break;
		}
	}break;
	case'f': {
		switch (reg[1]) {//fp
		case'p':str = "11110"; break;
		default: {//浮点数寄存器
			int num = 0;
			num += reg[1] - '0';
			if (reg.size() > 2) {
				num *= 10;
				num += reg[2] - '0';
			}
			str = Int2Bin(num, 5);
			break;
		    }
		}
	case 's': {
		switch (reg[1]) {
		case '0':str = "10000"; break;
		case '1':str = "10001"; break;
		case '2':str = "10010"; break;
		case '3':str = "10011"; break;
		case '4':str = "10100"; break;
		case '5':str = "10101"; break;
		case '6':str = "10110"; break;
		case '7':str = "10111"; break;
		case 'p':str = "11101"; break;
		}
	}break;
	case 'g':str = "11100"; break;
	case 'r':str = "11111"; break;
	}
	return str;
}

string Bin2HexChar(int v) {
	string s = "";
	switch (v) {
	case 0:s = "0"; break;
	case 1:s = "1"; break;
	case 2:s = "2"; break;
	case 3:s = "3"; break;
	case 4:s = "4"; break;
	case 5:s = "5"; break;
	case 6:s = "6"; break;
	case 7:s = "7"; break;
	case 8:s = "8"; break;
	case 9:s = "9"; break;
	case 10:s = "a"; break;
	case 11:s = "b"; break;
	case 12:s = "c"; break;
	case 13:s = "d"; break;
	case 14:s = "e"; break;
	case 15:s = "f"; break;
	}
	return s;
}

string Bin2Hex(string s) {
	string result = "0x";
	string temp4 = "";
	int t = 0;
	for (int i = 0; i < s.size(); i++) {
		temp4 += s[i];
		if (i % 4 == 3) {
			t = 0;
			for (int j = 0; j < 4; j++) {
				t *= 2;
				t += temp4[j] - '0';
			}
			result += Bin2HexChar(t);
			temp4 = "";
		}
	}
	return result;
}

string Bin2Int(string s) {
	int t = 0;
	for (int i = 0; i < s.size(); i++) {
		t *= 2;
		t += s[i] - '0';
	}
	int r;
	string rs = "";
	char temp;
	for (; t > 0; t /= 10) {
		r = t % 10;
		temp = r + '0';
		rs = temp + rs;
	}
	if (rs == "")
		rs = "0";
	return rs;
}

string HexChar2Bin(const char c)
{
	string temp;
	switch (c)
	{
	case '0':
		temp = "0000";
		break;
	case '1':
		temp = "0001";
		break;
	case '2':
		temp = "0010";
		break;
	case '3':
		temp = "0011";
		break;
	case '4':
		temp = "0100";
		break;
	case '5':
		temp = "0101";
		break;
	case '6':
		temp = "0110";
		break;
	case '7':
		temp = "0111";
		break;
	case '8':
		temp = "1000";
		break;
	case '9':
		temp = "1001";
		break;
	case 'A':
	case 'a':
		temp = "1010";
		break;
	case 'B':
	case 'b':
		temp = "1011";
		break;
	case 'C':
	case 'c':
		temp = "1100";
		break;
	case 'D':
	case 'd':
		temp = "1101";
		break;
	case 'E':
	case 'e':
		temp = "1110";
		break;
	case 'F':
	case 'f':
		temp = "1111";
		break;
	default:
		break;
	}

	return temp;
}

string Hex2Bin(string s, int k) {
	string tmp = "", val = "";
	int start;
	for (int i = 2; i < s.size(); i++) {
		tmp += HexChar2Bin(s[i]);
	}
	if (k == 32) {
		return tmp;
	}
	else {
		start = tmp.size() - k;
		for (int i = start; i >= 0 && i < tmp.size(); i++)
			val += tmp[i];
		return val;
	}
}
