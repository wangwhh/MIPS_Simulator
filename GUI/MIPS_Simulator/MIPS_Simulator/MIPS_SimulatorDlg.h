
// MIPS_SimulatorDlg.h: 头文件
//

#pragma once


// CMIPSSimulatorDlg 对话框
class CMIPSSimulatorDlg : public CDialogEx
{
// 构造
public:
	CMIPSSimulatorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIPS_SIMULATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();
	CListCtrl register_list;
	CListCtrl memory_list;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void UpdateRegisters();
	afx_msg void UpdateMemory();
	CString cur_file;
	CString cur_execute;
	CEdit edit1_ctrl;
	CEdit edit2_ctrl;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
};
