#pragma once
#include "afxdialogex.h"


// CTabReg 对话框

class CTabReg : public CDialogEx
{
	DECLARE_DYNAMIC(CTabReg)

public:
	CTabReg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabReg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void UpdateRegisters();
	CListCtrl register_list;
};
