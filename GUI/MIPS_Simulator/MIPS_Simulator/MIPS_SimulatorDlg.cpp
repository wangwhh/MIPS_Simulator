
// MIPS_SimulatorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MIPS_Simulator.h"
#include "code/assembler.h"
#include "code/data_transform.h"
#include "code/invert_asm.h"
#include "code/machine_code_execute.h"
#include "MIPS_SimulatorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern vector<string> memory;
extern vector<string> asm_codes;
extern int PC;
extern int cur;
string file_path;
int file_opened = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnAbout();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(19, &CAboutDlg::OnAbout)
END_MESSAGE_MAP()


// CMIPSSimulatorDlg 对话框



CMIPSSimulatorDlg::CMIPSSimulatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIPS_SIMULATOR_DIALOG, pParent)
	, cur_file(_T(""))
	, cur_execute(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMIPSSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_LIST_MEMORY, memory_list);
	DDX_Control(pDX, IDC_EDIT1, edit1_ctrl);
	DDX_Control(pDX, IDC_EDIT2, edit2_ctrl);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMIPSSimulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(32780, &CMIPSSimulatorDlg::OnOpen)
	ON_BN_CLICKED(IDC_BUTTON3, &CMIPSSimulatorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMIPSSimulatorDlg::OnBnClickedButton2)
	ON_COMMAND(ID_CLOSE, &CMIPSSimulatorDlg::OnClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMIPSSimulatorDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMIPSSimulatorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMIPSSimulatorDlg 消息处理程序

BOOL CMIPSSimulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);

	//ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	exeInitialize();
	CRect tabRect;
	m_tab.GetClientRect(&tabRect);
	tabRect.DeflateRect(2, 30, 5, 5);
	m_tab.InsertItem(0, _T("Registers"));
	m_tab.InsertItem(1, _T("Float"));
	tab_reg = new CTabReg();
	tab_float = new CTabFloat();
	tab_reg->Create(IDD_DIALOG_REG, &m_tab);
	tab_float->Create(IDD_DIALOG_FLT, &m_tab);
	tab_reg->MoveWindow(tabRect);
	tab_float->MoveWindow(tabRect);
	tab_reg->ShowWindow(SW_SHOW);
	tab_float->ShowWindow(SW_HIDE);
	//UpdateRegisters();

	// Memory
	CRect listRect;
	memory_list.GetClientRect(&listRect);
	memory_list.SetExtendedStyle(memory_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//设置列
	memory_list.InsertColumn(0, _T("Address"), LVCFMT_CENTER, listRect.Width() / 6, 0);
	memory_list.InsertColumn(1, _T("Machine Code"), LVCFMT_CENTER, listRect.Width() / 3, 1);
	memory_list.InsertColumn(2, _T("Assembler Code"), LVCFMT_CENTER, listRect.Width() / 2, 2);
	//设置行
	for (int i = 0; i < 65536; i += 4) {
		CString str;
		str.Format(_T("0x%04X"), i);
		memory_list.InsertItem(i / 4, str);
		str = Bin2Hex(memory[i] + memory[i + 1] + memory[i + 2] + memory[i + 3]).c_str();
		memory_list.SetItemText(i / 4, 1, str);
		if (i / 4 < asm_codes.size()) str = asm_codes[i / 4].c_str();
		else str = "";
		memory_list.SetItemText(i / 4, 2, str);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMIPSSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMIPSSimulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMIPSSimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMIPSSimulatorDlg::OnOpen()
{
	// TODO: 在此添加命令处理程序代码
	// 设置过滤器   
	TCHAR szFilter[] = _T("汇编文件 (*.asm)|*.asm|二进制文件 (*.bin)|*.bin|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, 0, NULL, 0, szFilter, this);

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		file_opened = 1;
		cur_file = fileDlg.GetPathName();
		edit1_ctrl.SetWindowTextA(cur_file);
		file_path = cur_file;
		if (file_path.substr(file_path.length() - 4, 4) == ".asm") cur_execute = "汇编";
		else if (file_path.substr(file_path.length() - 4, 4) == ".bin") cur_execute = "反汇编";
		edit2_ctrl.SetWindowTextA(cur_execute);
	}
	exeInitialize();
	tab_reg->UpdateRegisters();
	tab_float->UpdateRegisters();
	UpdateMemory();
	memory_list.SetFocus();
	memory_list.SetItemState(PC / 4, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void CAboutDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
}


void CMIPSSimulatorDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	exeInitialize();
	tab_reg->UpdateRegisters();
	tab_float->UpdateRegisters();
	UpdateMemory();
	memory_list.SetFocus();
	memory_list.SetItemState(PC/4, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void CMIPSSimulatorDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	cur = PC;
	PC+=4;
	Execute_instruction(memory[cur]+ memory[cur+1]+memory[cur+2]+memory[cur+3]);
	tab_reg->UpdateRegisters();
	tab_float->UpdateRegisters();
	UpdateMemory();
	memory_list.SetFocus();
	memory_list.SetItemState(PC/4, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}


void CMIPSSimulatorDlg::UpdateMemory()
{
	for (int i = 0; i < 65536; i += 4) {
		CString str;
		str = Bin2Hex(memory[i] + memory[i + 1] + memory[i + 2] + memory[i + 3]).c_str();
		memory_list.SetItemText(i / 4, 1, str);
		if (i / 4 < asm_codes.size()) str = asm_codes[i / 4].c_str();
		else str = "";
		memory_list.SetItemText(i / 4, 2, str);
	}
}

void CMIPSSimulatorDlg::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	exit (0);
}


void CMIPSSimulatorDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	while (PC / 4 < asm_codes.size()) {
		OnBnClickedButton2();
	}
}


void CMIPSSimulatorDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_tab.GetCurSel()) {
	case 0:
		tab_reg->ShowWindow(SW_SHOW);
		tab_float->ShowWindow(SW_HIDE);
		break;
	case 1:
		tab_reg->ShowWindow(SW_HIDE);
		tab_float->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
