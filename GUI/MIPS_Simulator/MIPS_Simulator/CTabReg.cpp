// CTabReg.cpp: 实现文件
//

#include "pch.h"
#include "MIPS_Simulator.h"
#include "code/data_transform.h"
#include "code/machine_code_execute.h"
#include "afxdialogex.h"
#include "CTabReg.h"

extern string register_name[];
extern vector<string> registers;
extern int PC;
// CTabReg 对话框

IMPLEMENT_DYNAMIC(CTabReg, CDialogEx)

CTabReg::CTabReg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REG, pParent)
{

}

CTabReg::~CTabReg()
{
}

void CTabReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REGISTER, register_list);
}

BOOL CTabReg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Register
	register_list.SetExtendedStyle(register_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//设置列
	register_list.InsertColumn(0, _T("Register"), LVCFMT_CENTER, 100, 0);
	register_list.InsertColumn(1, _T("Hexadecimal"), LVCFMT_CENTER, 300, 1);
	register_list.InsertColumn(2, _T("Decimal"), LVCFMT_CENTER, 200, 2);
	//设置行
	register_list.InsertItem(0, "pc");
	for (int i = 1; i < 33; i++) {
		register_list.InsertItem(i, register_name[i - 1].c_str());
	}
	UpdateRegisters();
	return TRUE;
}

void CTabReg::UpdateRegisters()
{
	register_list.SetItemText(0, 1, Bin2Hex(Int2Bin(PC, 16)).c_str());
	register_list.SetItemText(0, 2, to_string(PC).c_str());
	for (int i = 1; i <= 32; i++) {
		register_list.SetItemText(i, 1, Bin2Hex(registers[i - 1]).c_str());
		register_list.SetItemText(i, 2, Bin2Int(registers[i - 1]).c_str());
	}
}

BEGIN_MESSAGE_MAP(CTabReg, CDialogEx)
END_MESSAGE_MAP()


// CTabReg 消息处理程序
