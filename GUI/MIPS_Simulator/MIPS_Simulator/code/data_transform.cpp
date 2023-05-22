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
	case 'k': {
		switch (reg[1]) {
		case '0':str = "11010"; break;
		case '1':str = "10011"; break;
		}
	}break;
	case 'f': {
		switch (reg[1]) {//fp
		case 'p':str = "11110"; break;
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
	}break;
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
		case '8':str = "11110"; break;
		case 'p':str = "11101"; break;
		}
	}break;
	case 'g':str = "11100"; break;
	case 'r':str = "11111"; break;
	case 'i':str = "00001"; break;
	case 'h':str = "11111"; break;
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

	for (unsigned long long i = 0; i < mantissaString.length(); ++i)
		if (mantissaString[i] == '1')
		{
			mantissa += 1.0 / pow(2, i + 1);
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
	if (s[0] == '0') num = 0;
	else num = -1;
	for (int i = 0; i < s.size(); i++)
	{
		num = num << 1;
		if (s[i] == '1') num++;
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

