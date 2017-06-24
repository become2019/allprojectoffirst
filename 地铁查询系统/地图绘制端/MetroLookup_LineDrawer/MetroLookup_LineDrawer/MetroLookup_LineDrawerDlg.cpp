
// MetroLookup_LineDrawerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MetroLookup_LineDrawer.h"
#include "MetroLookup_LineDrawerDlg.h"
#include "afxdialogex.h"
#include "MDDlg.h"
#include "NewLineDlg.h"
#include "AddStationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMLLDDlg 对话框



CMLLDDlg::CMLLDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_METROLOOKUP_LINEDRAWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMLLDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LINE_COUNT, m_static_linecount);
}

BEGIN_MESSAGE_MAP(CMLLDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_LOAD_MAP, &CMLLDDlg::OnBnClickedMfcbuttonLoadMap)
	ON_BN_CLICKED(IDC_MFCBUTTON_ADD_STATION, &CMLLDDlg::OnBnClickedMfcbuttonAddStation)
	ON_BN_CLICKED(IDC_MFCBUTTON_NEW_LINE, &CMLLDDlg::OnBnClickedMfcbuttonNewLine)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CMLLDDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_MFCBUTTON_SAVE_MAP, &CMLLDDlg::OnBnClickedMfcbuttonSaveMap)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MFCBUTTON_LOAD_BLUEPRINT, &CMLLDDlg::OnBnClickedMfcbuttonLoadBlueprint)
	ON_BN_CLICKED(IDC_BUTTON_ADD_END, &CMLLDDlg::OnBnClickedButtonAddEnd)
	ON_BN_CLICKED(IDC_BUTTON_ADD_BEG_STATION, &CMLLDDlg::OnBnClickedButtonAddBegStation)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CMLLDDlg::OnBnClickedButtonUndo)
END_MESSAGE_MAP()


// CMLLDDlg 消息处理程序

BOOL CMLLDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	mddlg = new CMDDlg;
	mddlg->Create(IDD_DIALOG_MAP_DRAWER, GetDlgItem(IDC_TAB_MAP_FRAME));
	CRect TabRect;
	GetDlgItem(IDC_TAB_MAP_FRAME)->GetClientRect(TabRect);
	mddlg->SetWindowPos(NULL, TabRect.left, TabRect.top + 21, TabRect.Width() - 3, TabRect.Height() - 23, SWP_SHOWWINDOW);
	GetDlgItem(IDC_MFCBUTTON_SAVE_MAP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_BEG_STATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_END)->EnableWindow(FALSE);
	CMLLDDlg::OnBnClickedButtonLock();
	//首先得到数据库文件地址
	CString dbpath, dbstr = _T("");
	CStdioFile csf;
	csf.Open(_T("config.cfg"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	csf.ReadString(dbpath);
	csf.Close();
	dbstr = _T("Provider=Microsoft.ACE.OLEDB.12.0;\
Data Source=") + dbpath + _T(";");
	//连接数据库
	try
	{
		CoInitialize(NULL);
		m_pConnection = _ConnectionPtr(__uuidof(Connection));
		m_pConnection->ConnectionString = dbstr.GetBuffer(0);//这里照着老版的书上写Provider=Microsoft.Jet.OLEDB.4.0;Data
												//Source= MyAccess.mdb是不对的，这样写只适合2007版以前的access，且路径里面的\必须改成
		m_pConnection->Open("", "", "", adConnectUnspecified);
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败！"));
		return FALSE;
	}
	//得到已经存在的记录条数，站点ID将会以这个来设置
	_variant_t rca;
	m_pRecordset.CreateInstance(_T("ADODB.Recordset"));
	m_pRecordset = m_pConnection->Execute(_T("SELECT COUNT(*)FROM STATION_DATA"), &rca, adCmdText);
	_variant_t vvt = (long)0;
	_variant_t vvc = m_pRecordset->GetCollect(vvt);
	CString fxxx;
	LineStationCount = vvc.lVal;
	//得到有多少条线路
	if (!KSQL_Excute(_T("SELECT LINECOUNT FROM LINECOUNT WHERE ID=3"),m_pRecordset))
	{
		AfxMessageBox(_T("数据库读取错误"));
	}
	if (!KSQL_CheckRecordSetGotValue(m_pRecordset))
	{
		AfxMessageBox(_T("数据库读取错误:未查询到指定数据"));
		LineConut = 0;
	}
	else
	{
		vvc = m_pRecordset->GetCollect(_variant_t((long)0));
		LineConut = vvc.lVal;
	}
	fxxx.Format(_T("站点数量: %d\n线路数量: %d"), LineStationCount, LineConut);
	m_pRecordset->Close();
	//数据库连接代码完毕
	m_static_linecount.SetWindowTextW(fxxx);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMLLDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMLLDDlg::OnPaint()
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
HCURSOR CMLLDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMLLDDlg::OnBnClickedMfcbuttonLoadMap()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Metro Line Data Files (*.mld)|*.mld|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
		filename = FilePathName;

		filename = filename.MakeReverse();
		filename = filename.Mid(0, filename.Find(_T("\\"), 0));
		filename = filename.MakeReverse();
		GetDlgItem(IDC_MFCBUTTON_NEW_LINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON_SAVE_MAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON_LOAD_MAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(TRUE);

		filename = FilePathName;
		NO_DRAW = false;
		CMLLDDlg::OnBnClickedButtonLock();
		mddlg->OnOpenExistFile(filename);

		return;
	}
	else
	{
		return;
	}

}


void CMLLDDlg::OnBnClickedMfcbuttonAddStation()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddStationDlg dlg;
	dlg.DoModal();
}



void CMLLDDlg::OnBnClickedMfcbuttonNewLine()
{
	// TODO: 在此添加控件通知处理程序代码
	CNewLineDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MHE.bound = 3;
		CRect TabRect;
		GetDlgItem(IDC_TAB_MAP_FRAME)->GetClientRect(TabRect);
		mddlg->SetWindowPos(NULL, TabRect.left, TabRect.top + 21, TabRect.Width() - 3, TabRect.Height() - 23, SWP_SHOWWINDOW);
		GetDlgItem(IDC_MFCBUTTON_NEW_LINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON_SAVE_MAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBUTTON_LOAD_MAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ADD_BEG_STATION)->EnableWindow(FALSE);
		NO_DRAW = false;
		CFile fff;
		fff.Open(STRING_BUFFER_FILENEMA, CFile::modeCreate | CFile::modeWrite);
		fff.Close();
		MessageBoxW(_T("----------------------------\n为了防止误操作，绘制锁定默认开启\n----------------------------"), _T("提示"), MB_ICONEXCLAMATION | MB_OK);
	}
}

const MMD_HEAD CMLLDDlg::GetHead()
{

	return MHE;
}


const CString CMLLDDlg::GetFileName()
{
	return filename;
}

const STATION_DATA CMLLDDlg::GetStationData()
{
	return SD;
}

void CMLLDDlg::SetSatationData(const STATION_DATA sdta)
{
	SD = sdta;
}

const bool CMLLDDlg::IsAddStation()
{
	return add_station;
}

const bool CMLLDDlg::IsAddBegStation()
{
	return GV_BOOL_BEG_STATION_ADDED;
}

const bool CMLLDDlg::IsAddEndStation()
{
	return GV_BOOL_END_STATION_ADDED;
}

void CMLLDDlg::SetAddBegStationStatus(const bool b)
{
	GV_BOOL_BEG_STATION_ADDED = b;
}

void CMLLDDlg::SetAddEndStationStatus(const bool b)
{
	GV_BOOL_END_STATION_ADDED = b;
}

bool CMLLDDlg::SetAddStationStatus(const bool status)
{
	add_station = status;
	return add_station;
}

void CMLLDDlg::LockAll()
{
	GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_BEG_STATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBUTTON_LOAD_MAP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBUTTON_NEW_LINE)->EnableWindow(FALSE);
}

void CMLLDDlg::SetTransferStationStatus(const bool st)
{
	add_station_is_transfer = st;
}

const bool CMLLDDlg::IsTransferStation()
{
	return add_station_is_transfer;
}

bool CMLLDDlg::IsDrawLock()
{
	return draw_lock;
}

bool CMLLDDlg::NoDrawStatus()
{
	return NO_DRAW;
}

const bool CMLLDDlg::KSQL_Excute(const CString cmd, _RecordsetPtr & rp)
{
	CString CMD = cmd;
		try
		{
			rp = GetConnectionPtr()->Execute(CMD.GetBuffer(0), NULL, adCmdText);
		}
		catch (_com_error e)
		{
			MessageBoxW(_T("错误信息：") + e.Description(), _T("数据库读取错误"), MB_OK | MB_ICONERROR);
			return false;
		}
	return true;
}
const bool CMLLDDlg::KSQL_Excute(const CString cmd)
{
	CString CMD = cmd;
		try
		{
			GetConnectionPtr()->Execute(CMD.GetBuffer(0), NULL, adCmdText);
		}
		catch (_com_error e)
		{
			MessageBoxW(_T("错误信息：") + e.Description(), _T("数据库读取错误"), MB_OK | MB_ICONERROR);
			return false;
		}
	return true;
}

const bool CMLLDDlg::KSQL_CheckRecordSetGotValue(const _RecordsetPtr rp)
{
	_variant_t vt;
	try
	{
		vt = rp->GetCollect((long)0);  //只判断第一记录集
	}
	catch (_com_error e)
	{
		return false;
	}
	return true;
}

const _ConnectionPtr CMLLDDlg::GetConnectionPtr()
{
	return m_pConnection;
}

const _RecordsetPtr CMLLDDlg::GetRecordsetPtr()
{
	return m_pRecordset;
}

const int CMLLDDlg::GetLineStationCount()
{
	return LineStationCount;
}

const int CMLLDDlg::GetLineConut()
{
	return LineConut;
}

void CMLLDDlg::SetFileName(const CString fn)
{
	filename = fn;
}

void CMLLDDlg::SetStaBegName(const CString ne)
{
	STA_NAME_BEG = ne;
}

void CMLLDDlg::SetStaEndName(const CString ne)
{
	STA_NAME_END = ne;
}

void CMLLDDlg::SetTermPrice(const int price)
{
	STA_TERM_PRICE = price;
}

void CMLLDDlg::SetLineColor(const COLORREF clr)
{
	MHE.RGB[0] = GetRValue(clr);
	MHE.RGB[1] = GetGValue(clr);
	MHE.RGB[2] = GetBValue(clr);
}

const CString CMLLDDlg::GetStaBegName()
{
	return STA_NAME_BEG;
}

const CString CMLLDDlg::GetStaEndName()
{
	return STA_NAME_END;
}

const CString CMLLDDlg::GetTermPrice()
{
	CString bufx;
	bufx.Format(_T("%d"), STA_TERM_PRICE);
	return bufx;
}


void CMLLDDlg::OnBnClickedButtonLock()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!draw_lock)
	{
		SetDlgItemTextW(IDC_BUTTON_LOCK, _T("解锁绘制"));
		draw_lock = true;
		GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADD_BEG_STATION)->EnableWindow(FALSE);
	}
	else
	{
		SetDlgItemTextW(IDC_BUTTON_LOCK, _T("绘制锁定"));
		draw_lock = false;
		if (GV_BOOL_BEG_STATION_ADDED)
		{
			GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(TRUE);
			
		}
		if (!GV_BOOL_BEG_STATION_ADDED)
		{
			GetDlgItem(IDC_BUTTON_ADD_BEG_STATION)->EnableWindow(TRUE);
		}
	}
}


void CMLLDDlg::OnBnClickedMfcbuttonSaveMap()
{
	CStdioFile csflog;
	csflog.Open(_T("db_update_log.txt"), CFile::modeCreate | CFile::modeWrite);
	csflog.WriteString(_T("start update datebase\n"));
	GetDlgItem(IDC_MFCBUTTON_SAVE_MAP)->EnableWindow(FALSE);
	//首先从我们将数据库中的线路计数+1
	KSQL_Excute(_T("UPDATE LINECOUNT SET LINECOUNT=LINECOUNT+1"));
	CString buf;
	LineConut++;
	buf.Format(_T("%d,'"), LineConut);
	//然后从line.cache文件中读取站点数据，然后写入数据库
	CFile ff;
	ff.Open(STRING_BUFFER_FILENEMA, CFile::modeReadWrite);
	MMD_HEAD mhb;
	ff.Read(&mhb, sizeof(MMD_HEAD));
	//然后，我们把头数据写入数据库，接下来读取地图体
	CString SQLstr = _T("INSERT INTO LINE_DATA(LINEID,RGB_R,RGB_G,RGB_B) "), SQLval = _T("");
	SQLval.Format(_T(" VALUES(%d,%d,%d,%d) "), LineConut,mhb.RGB[0], mhb.RGB[1], mhb.RGB[2]);
	SQLstr += SQLval;
	csflog.WriteString(_T("UPDATE line_data : SQLstr = \n")+SQLstr +_T("\n"));
	KSQL_Excute(SQLstr);
	MMD_NODE mnb;
	while (ff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		//更新入数据库
		SQLstr = _T(""), SQLval = _T("");
		SQLstr = _T("INSERT INTO STATION_DATA(ID,STANAME,LINE,LTIME,RTIME,LPRICE,RPRICE,POS_X,POS_Y,TRANSFER) ");
		SQLval.Format(_T(" VALUES(%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d) "),mnb.id,CString(mnb.name),mnb.line,mnb.ltime,mnb.rtime,mnb.lprice,mnb.rprice,mnb.p.x,mnb.p.y,(int)mnb.transfer);
		SQLstr += SQLval;
		csflog.WriteString(_T("UPDATE station_data : SQLstr= \n")+SQLstr+_T("\n"));
		KSQL_Excute(SQLstr);
		//如果是换乘站点
		if (mnb.transfer)
		{
				////首先判断是否存在同名的站点,如果不存在则在TRANSFER_STATION中添加，然后再添加相应的线路ID
				//如果存在，则查询当前站点在TRANSFER_STATION中是否存在当前线路ID
				CString SQLstr;
				CString LINEIDBUF;
				LINEIDBUF.Format(_T("%d"), mnb.line);
				SQLstr = _T("SELECT * FROM TRANSFER_STATION WHERE STANAME='") + CString(mnb.name) + _T("'");
				int iib = 0;
				int x = 1;
				m_pRecordset.CreateInstance(_T("ADODB.Recordset"));
				try {
					m_pRecordset = GetConnectionPtr()->Execute(SQLstr.GetBuffer(0), NULL, adCmdText);
					_variant_t vt = m_pRecordset->GetCollect((long)0);
					iib = (int)vt.lVal;
				}
				catch (_com_error e)   //如果出错，就说明该站点目前是第一次添加为换乘站点
				{
					SQLstr = _T("INSERT INTO TRANSFER_STATION(STANAME,PARENTLINE) VALUES('") + CString(mnb.name) + _T("','") + LINEIDBUF + _T("')");
					GetConnectionPtr()->Execute(SQLstr.GetBuffer(0), NULL, adCmdText);
					x = 0;
				}
				if (x) //TRANSFER_STATION中已经存在相同名字的换乘站点，那么我们判断当前站点的线路ID是否在PARENTLINE字段中
				{
					SQLstr = _T("SELECT PARENTLINE FROM TRANSFER_STATION WHERE STANAME='") + CString(mnb.name) + _T("'");
					m_pRecordset = GetConnectionPtr()->Execute(SQLstr.GetBuffer(0), NULL, adCmdText);
					_variant_t vt = m_pRecordset->GetCollect((long)0);
					CString PLD = (LPCTSTR)vt.lVal;
					if (PLD.Find(LINEIDBUF) < 0)  //未找到，说明我们需要添加到数据库中
					{
						CString LXL = PLD + LINEIDBUF;
						SQLstr = _T("UPDATE TRANSFER_STATION SET PARENTLINE = '") + LXL + _T("' WHERE PARENTLINE= '") + PLD + _T("'");
						try
						{
							GetConnectionPtr()->Execute(SQLstr.GetBuffer(0), NULL, adCmdText);
						}
						catch (_com_error e)
						{
							MessageBoxW(_T("更新数据库失败\n错误原因：") + e.Description(), _T("错误"), MB_ICONERROR | MB_OK);
						}
					}
				}
		}
	}
	ff.Close();
	//更新统计数据的显示
	int a = 0, b = 0;
	KSQL_Excute(_T("SELECT COUNT(*)FROM STATION_DATA"),m_pRecordset);
	_variant_t vvc = m_pRecordset->GetCollect(_variant_t((long)0));
	a = (int)vvc.lVal;
	KSQL_Excute(_T("SELECT LINECOUNT FROM LINECOUNT WHERE ID=3"), m_pRecordset);
	vvc = m_pRecordset->GetCollect(_variant_t((long)0));
	m_pConnection->Close();
	b = (int)vvc.lVal;
	CString fxxx;
	fxxx.Format(_T("站点数量: %d\n线路数量: %d"), a, b);
	m_static_linecount.SetWindowTextW(fxxx);
	HAVE_SYNC = true;

	csflog.Close();
}


void CMLLDDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (NO_DRAW)
	{
		CDialogEx::OnClose();
	}
	else
	{
		if (!HAVE_SYNC)
		{
			if (MessageBoxW(_T("地图文件已经建立或修改但尚未同步，确定退出？\n未同步则不会起效！"), _T("提示"), MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK)
			{
				CDialogEx::OnClose();
			}
		}
		else
		{
			CDialogEx::OnClose();
		}
	}
}


void CMLLDDlg::OnBnClickedMfcbuttonLoadBlueprint()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("蓝图文件 (*.mmld)|*.mmld|蓝图文件 (*.cache)|*.cache||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //得到蓝图路径
		mddlg->OnOpenExistFile(FilePathName, true);
		return;
	}
	else
	{
		return;
	}
}


void CMLLDDlg::OnBnClickedButtonAddEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	GV_BOOL_END_STATION_ADDED = true;
	GetDlgItem(IDC_BUTTON_ADD_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBUTTON_SAVE_MAP)->EnableWindow(TRUE);
	//(CMFCButton*)GetDlgItem(IDC_MFCBUTTON_SAVE_MAP)->SetBack
}


void CMLLDDlg::OnBnClickedButtonAddBegStation()
{
	// TODO: 在此添加控件通知处理程序代码
	GV_BOOL_BEG_STATION_ADDED = true;
	GetDlgItem(IDC_BUTTON_ADD_BEG_STATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_END)->EnableWindow(TRUE);
	GetDlgItem(IDC_MFCBUTTON_ADD_STATION)->EnableWindow(TRUE);
}


void CMLLDDlg::OnBnClickedButtonUndo()
{
	// TODO: 在此添加控件通知处理程序代码
	mddlg->Undo();
}
