#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include"data_transform.h"

static vector<string> binarys;
static string buffer;
static map<string, int> Label;
static int addr = 0;

void MIPSassembler();
void GetLabel();

#endif