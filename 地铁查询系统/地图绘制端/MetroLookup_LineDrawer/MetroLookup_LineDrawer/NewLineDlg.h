#pragma once
#include "afxcolorbutton.h"


// CNewLineDlg 对话框

class CNewLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewLineDlg)

public:
	CNewLineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewLineDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
private:
	const CString STR_FILENAME = _T("请输入文件名");
	const CString STR_STABEG = _T("请输入起点站");
	const CString STR_STAEND = _T("请输入终点站名");
	const CString STR_PRICE = _T("请输入全程票价");
public:
	afx_msg void OnEnSetfocusEditNewLineBegStation();
	afx_msg void OnEnSetfocusEditNewLineEndStation();
	afx_msg void OnEnSetfocusEditNewLinePrice();
	afx_msg void OnEnKillfocusEditNewLineBegStation();
	afx_msg void OnEnKillfocusEditNewLineEndStation();
	afx_msg void OnEnKillfocusEditNewLinePrice();
	CMFCColorButton m_clolor_new_line;
};
