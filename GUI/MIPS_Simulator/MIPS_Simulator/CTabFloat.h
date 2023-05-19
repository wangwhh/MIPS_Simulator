#pragma once
#include "afxdialogex.h"

// CTabFloat 对话框

class CTabFloat : public CDialogEx
{
	DECLARE_DYNAMIC(CTabFloat)

public:
	CTabFloat(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabFloat();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FLT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl float_list;
	afx_msg void UpdateRegisters();
};
