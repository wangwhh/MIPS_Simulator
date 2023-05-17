#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

void Disassemble(string file_path);
string invert_line(string buffer);
string get_register(string reg);
