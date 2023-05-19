// CTabFloat.cpp: 实现文件
//

#include "pch.h"
#include "MIPS_Simulator.h"
#include "code/data_transform.h"
#include "CTabFloat.h"
#include "afxdialogex.h"
#include "code/machine_code_execute.h"

extern vector<string> f_registers;

// CTabFloat 对话框

IMPLEMENT_DYNAMIC(CTabFloat, CDialogEx)

CTabFloat::CTabFloat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FLT, pParent)
{

}

CTabFloat::~CTabFloat()
{
}

void CTabFloat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FLOAT, float_list);
}

BOOL CTabFloat::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Register
	float_list.SetExtendedStyle(float_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//设置列
	float_list.InsertColumn(0, _T("Register"), LVCFMT_CENTER, 100, 0);
	float_list.InsertColumn(1, _T("Hexadecimal"), LVCFMT_CENTER, 300, 1);
	float_list.InsertColumn(2, _T("Decimal"), LVCFMT_CENTER, 200, 2);
	//设置行
	string f_register_name[32] = { "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10",
	"f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24",
	"f25", "f26", "f27", "f28", "f29", "f30", "f31" };
	for (int i = 0; i < 48; i++) {
		if (i % 3 == 0)
			float_list.InsertItem(i, f_register_name[i * 2 / 3].c_str());
		else if (i % 3 == 1)
			float_list.InsertItem(i, f_register_name[(i - 1) * 2 / 3 + 1].c_str());
		else
			float_list.InsertItem(i, (f_register_name[(i - 2) * 2 / 3] + "+" + f_register_name[(i - 2) * 2 / 3 + 1]).c_str());
	}
	UpdateRegisters();
	return TRUE;
}

void CTabFloat::UpdateRegisters()
{
	for (int i = 0; i < 48; i++) {
		if (i % 3 == 0) {
			float_list.SetItemText(i, 1, Bin2Hex(f_registers[i * 2 / 3]).c_str());
			float_list.SetItemText(i, 2, to_string(Bin2Float(f_registers[i * 2 / 3])).c_str());
		}
			
		else if (i % 3 == 1) {
			float_list.SetItemText(i, 1, Bin2Hex(f_registers[(i - 1) * 2 / 3 + 1]).c_str());
			float_list.SetItemText(i, 2, to_string(Bin2Float(f_registers[(i - 1) * 2 / 3 + 1])).c_str());
		}
		else {
			float_list.SetItemText(i, 1, Bin2Hex(f_registers[(i - 2) * 2 / 3] + f_registers[(i - 2) * 2 / 3 + 1]).c_str());
			float_list.SetItemText(i, 2, to_string(Bin2Double(f_registers[(i - 2) * 2 / 3], f_registers[(i - 2) * 2 / 3 + 1])).c_str());
		}
			
	}
}


BEGIN_MESSAGE_MAP(CTabFloat, CDialogEx)
END_MESSAGE_MAP()


// CTabFloat 消息处理程序
