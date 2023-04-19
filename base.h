//MIPS simulator (1 word = 4 bytes = 32 bits)
//The memory of MIPS is from 0x00000000 to 0xFFFFFFFF
//the addresses of memory available in the user state is kuseg(2GB)
//Operands in MIPS are divided into 3 categories:registers,memory,immediate numbers.
#include<iostream>
#include<vector>

using namespace std;

//Here are 16 General Purpose Registers below:
//$zero:always returns 0
//$at:for assembly when reorganizing of large constants
//$v0~$v1:for the result or return value when running the subroutine(other values use memory)
//$a0~$a3:pass the first four parameters to the subroutine(other parameters use stack)
//$t0~$t7:temporary registers,are often used
//$s0~$s7:saving registers,are often used
//$t8~$t9:the same as $t0~$t7
//$k0~$k1:reserved for operating system and exception handling
//$gp:global pointer
//$sp:stack pointer
//$fp:frame pointer
//$ra:for return address(the jal and lr instruction)
vector<char>zero(32),at(32),v0(32),v1(32),a0(32),a1(32),a2(32),a3(32),t0(32),t1(32),t2(32),t3(32),t4(32),t5(32),t6(32),t7(32),s0(32),s1(32),s2(32),s3(32),s4(32),s5(32),s6(32),s7(32),t8(32),t9(32),k0(32),k1(32),gp(32),sp(32),fp(32),ra(32);
vector<char>pc(32),lo(32),hi(32);
vector<char>mem(65536);//64 kb
vector<char>cur(32);//to store the current instruction

/*
int main(void)
{//for testing
	for(int i=0;i<32;i++)
	{
		cur[i]='0'+i%2;
		cout<<cur[i];
	}
	return 0;
}
*/
