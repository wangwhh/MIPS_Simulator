#ifndef DATA_TRANSFORM_H_INCLUDED
#define DATA_TRANSFORM_H_INCLUDED

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<map>
#include<fstream>
#include<sstream>
#include<math.h>

using namespace std;

string Int2Bin(int val, int k);
string Reg2Bin(string reg);
string HexChar2Bin(const char c);
string Bin2HexChar(int v);
string Bin2Hex(string s);
string Bin2Int(string s);
string Hex2Bin(string s, int k);
float Bin2Float(string s);
string Float2Bin(float value);
double Bin2Double(string s1, string s2);
string Double2Bin(double value);
int StrToSign(string s);
int StrToUnsign(string s);

#endif