// AddStationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MetroLookup_LineDrawer.h"
#include "AddStationDlg.h"
#include "afxdialogex.h"
#include "MetroLookup_LineDrawerDlg.h"


// CAddStationDlg 对话框

IMPLEMENT_DYNAMIC(CAddStationDlg, CDialogEx)

CAddStationDlg::CAddStationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ADD_STATION, pParent)
{

}

CAddStationDlg::~CAddStationDlg()
{
}

void CAddStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_IS_TRANSFER, m_checkbox);
}


BEGIN_MESSAGE_MAP(CAddStationDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT_STATION_NAME, &CAddStationDlg::OnEnSetfocusEditStationName)
	ON_EN_SETFOCUS(IDC_EDIT_STATION_LTIME, &CAddStationDlg::OnEnSetfocusEditStationLtime)
	ON_EN_SETFOCUS(IDC_EDIT_STATION_RTIMR, &CAddStationDlg::OnEnSetfocusEditStationRtimr)
	ON_EN_SETFOCUS(IDC_EDIT_STATION_PRICE_FROM_L, &CAddStationDlg::OnEnSetfocusEditStationPriceFromL)
	ON_EN_SETFOCUS(IDC_EDIT_STATION_PRICE_FROM_R, &CAddStationDlg::OnEnSetfocusEditStationPriceFromR)
	ON_EN_KILLFOCUS(IDC_EDIT_STATION_NAME, &CAddStationDlg::OnEnKillfocusEditStationName)
	ON_EN_KILLFOCUS(IDC_EDIT_STATION_LTIME, &CAddStationDlg::OnEnKillfocusEditStationLtime)
	ON_EN_KILLFOCUS(IDC_EDIT_STATION_RTIMR, &CAddStationDlg::OnEnKillfocusEditStationRtimr)
	ON_EN_KILLFOCUS(IDC_EDIT_STATION_PRICE_FROM_L, &CAddStationDlg::OnEnKillfocusEditStationPriceFromL)
	ON_EN_KILLFOCUS(IDC_EDIT_STATION_PRICE_FROM_R, &CAddStationDlg::OnEnKillfocusEditStationPriceFromR)
	ON_BN_CLICKED(IDOK, &CAddStationDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_STATION_PRICE_FROM_L, &CAddStationDlg::OnEnChangeEditStationPriceFromL)
END_MESSAGE_MAP()


// CAddStationDlg 消息处理程序


void CAddStationDlg::OnEnSetfocusEditStationName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_NAME, str);
	if (str == STRING_EDIT_STATION_NAME)
	{
		GetDlgItem(IDC_EDIT_STATION_NAME)->SetWindowTextW(_T(""));
	}
}


void CAddStationDlg::OnEnKillfocusEditStationName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_NAME, str);
	if (str == _T("") || str == _T(" "))
	{
		GetDlgItem(IDC_EDIT_STATION_NAME)->SetWindowTextW(STRING_EDIT_STATION_NAME);
	}
}


void CAddStationDlg::OnEnKillfocusEditStationLtime()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_LTIME, str);
	if (str == _T("") || str == _T(" "))
	{
		GetDlgItem(IDC_EDIT_STATION_LTIME)->SetWindowTextW(STRING_EDIT_LTIME);
	}
}


void CAddStationDlg::OnEnKillfocusEditStationRtimr()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_RTIMR, str);
	if (str == _T("") || str == _T(" "))
	{
		GetDlgItem(IDC_EDIT_STATION_RTIMR)->SetWindowTextW(STRING_EDIT_RTIME);
	}
}


void CAddStationDlg::OnEnKillfocusEditStationPriceFromL()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_L, str);
	if (str == _T("") || str == _T(" "))
	{
		GetDlgItem(IDC_EDIT_STATION_PRICE_FROM_L)->SetWindowTextW(STRING_EDIT_LPRICE);
	}
}


void CAddStationDlg::OnEnKillfocusEditStationPriceFromR()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_R, str);
	if (str == _T("") || str == _T(" "))
	{
		GetDlgItem(IDC_EDIT_STATION_PRICE_FROM_R)->SetWindowTextW(STRING_EDIT_RPRICE);
	}
}


void CAddStationDlg::OnEnSetfocusEditStationLtime()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_LTIME, str);
	if (str == STRING_EDIT_LTIME)
	{
		GetDlgItem(IDC_EDIT_STATION_LTIME)->SetWindowTextW(_T(""));
	}
}


void CAddStationDlg::OnEnSetfocusEditStationRtimr()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_RTIMR, str);
	if (str == STRING_EDIT_RTIME)
	{
		GetDlgItem(IDC_EDIT_STATION_RTIMR)->SetWindowTextW(_T(""));
	}
}


void CAddStationDlg::OnEnSetfocusEditStationPriceFromL()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_L, str);
	if (str == STRING_EDIT_LPRICE)
	{
		GetDlgItem(IDC_EDIT_STATION_PRICE_FROM_L)->SetWindowTextW(_T(""));
	}
}


void CAddStationDlg::OnEnSetfocusEditStationPriceFromR()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_R, str);
	if (str == STRING_EDIT_RPRICE)
	{
		GetDlgItem(IDC_EDIT_STATION_PRICE_FROM_R)->SetWindowTextW(_T(""));
	}
}


void CAddStationDlg::OnBnClickedOk()
{
	CMLLDDlg *mlldlg = (CMLLDDlg*)GetParent();
	// TODO: 在此添加控件通知处理程序代码
	CString station_name, l_time, r_time, l_price, r_price;
	GetDlgItemTextW(IDC_EDIT_STATION_NAME, station_name);
	GetDlgItemTextW(IDC_EDIT_STATION_LTIME, l_time);
	GetDlgItemTextW(IDC_EDIT_STATION_RTIMR, r_time);
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_L, l_price);
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_R, r_price);
	if (station_name.IsEmpty() || l_time.IsEmpty() || r_time.IsEmpty() || l_price.IsEmpty() || r_price.IsEmpty()||
		station_name==STRING_EDIT_STATION_NAME || l_time==STRING_EDIT_LTIME || r_time==STRING_EDIT_RTIME ||
		l_price==STRING_EDIT_LPRICE || r_price==STRING_EDIT_RPRICE)
	{
		MessageBox(_T("请输入站点名，站点耗时，站点票价等信息后重试！"), _T("错误"), MB_ICONERROR | MB_OK);
	}
	else
	{
		STATION_DATA SD;
		SD.name = station_name;
		SD.L_time = l_time;//_ttoi(l_time);
		SD.R_time = r_time;//_ttoi(r_time);
		SD.L_price = l_price;//_ttoi(l_price);
		SD.R_price = r_price;//_ttoi(r_price);
		mlldlg->SetSatationData(SD);
		mlldlg->SetAddStationStatus(true);
		if (m_checkbox.GetCheck() == TRUE)
		{
			mlldlg->SetTransferStationStatus(true);
		}
		CDialogEx::OnOK();
	}
}





BOOL CAddStationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemTextW(IDC_EDIT_STATION_NAME, STRING_EDIT_STATION_NAME);
	SetDlgItemTextW(IDC_EDIT_STATION_LTIME, STRING_EDIT_LTIME);
	SetDlgItemTextW(IDC_EDIT_STATION_RTIMR, STRING_EDIT_RTIME);
	SetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_L, STRING_EDIT_LPRICE);
	SetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_R, STRING_EDIT_RPRICE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAddStationDlg::OnEnChangeEditStationPriceFromL()
{
	// TODO:  在此添加控件通知处理程序代码
	CString dta;
	CMLLDDlg *mlldlg = (CMLLDDlg*)GetParent();
	CString tp = mlldlg->GetTermPrice();
	term_price = _ttoi(tp);
	GetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_L, dta);
	if (dta == STRING_EDIT_LPRICE || dta == _T(""))
	{
		return;
	}
	int lp = _ttoi(dta);
	lp = term_price - lp;
	CString sss;
	sss.Format(_T("%d"), lp);
	SetDlgItemTextW(IDC_EDIT_STATION_PRICE_FROM_R, sss);
}
