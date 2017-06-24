#pragma once
#include "afxwin.h"


// CAddStationDlg 对话框

class CAddStationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddStationDlg)

public:
	CAddStationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddStationDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_STATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocusEditStationName();
	afx_msg void OnEnSetfocusEditStationLtime();
	afx_msg void OnEnSetfocusEditStationRtimr();
	afx_msg void OnEnSetfocusEditStationPriceFromL();
	afx_msg void OnEnSetfocusEditStationPriceFromR();
	afx_msg void OnEnKillfocusEditStationName();
	afx_msg void OnEnKillfocusEditStationLtime();
	afx_msg void OnEnKillfocusEditStationRtimr();
	afx_msg void OnEnKillfocusEditStationPriceFromL();
	afx_msg void OnEnKillfocusEditStationPriceFromR();
	afx_msg void OnBnClickedOk();
private:
	const CString STRING_EDIT_STATION_NAME = _T("请输入站点名");
	const CString STRING_EDIT_LTIME = _T("左行到站耗时");
	const CString STRING_EDIT_RTIME = _T("右行到站耗时");
	const CString STRING_EDIT_LPRICE = _T("从起点站出发的票价");
	const CString STRING_EDIT_RPRICE = _T("从终点站出发的票价");
	int term_price = 0;
public:
	virtual BOOL OnInitDialog();
	CButton m_checkbox;
	afx_msg void OnEnChangeEditStationPriceFromL();
};
