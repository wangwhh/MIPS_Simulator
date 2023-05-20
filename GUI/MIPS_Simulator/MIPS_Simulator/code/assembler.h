#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include"data_transform.h"

static string buffer;

static int addr = 0;

void MIPSassembler(string file_path);
void GetLabel(string file_path);

#endif
