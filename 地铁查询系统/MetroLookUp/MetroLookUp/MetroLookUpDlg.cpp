
// MetroLookUpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MetroLookUp.h"
#include "MetroLookUpDlg.h"
#include "afxdialogex.h"

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


// CMLUDlg 对话框



CMLUDlg::CMLUDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_METROLOOKUP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMLUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_HIGHLIGHT, m_combobox);
}

BEGIN_MESSAGE_MAP(CMLUDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BUTTON_LOOK_UP, &CMLUDlg::OnBnClickedLookUp)
	ON_EN_SETFOCUS(EDIT_BEGIN, &CMLUDlg::OnEnSetfocusBegin)
	ON_EN_SETFOCUS(EDIT_END, &CMLUDlg::OnEnSetfocusEnd)
	ON_EN_KILLFOCUS(EDIT_BEGIN, &CMLUDlg::OnEnKillfocusBegin)
	ON_EN_KILLFOCUS(EDIT_END, &CMLUDlg::OnEnKillfocusEnd)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_MFCBUTTON_INFO_PRICE, &CMLUDlg::OnBnClickedMfcbuttonInfoPrice)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLEARHIGHLIGHT, &CMLUDlg::OnBnClickedButtonClearhighlight)
	ON_CBN_CLOSEUP(IDC_COMBO_HIGHLIGHT, &CMLUDlg::OnCbnCloseupComboHighlight)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CMLUDlg::OnBnClickedButtonReload)
END_MESSAGE_MAP()


// CMLUDlg 消息处理程序

BOOL CMLUDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	csflog.Open(_T("MLU_log.txt"), CFile::modeWrite | CFile::modeCreate);
	csflog.WriteString(_T("-----in-----[FUNCTION]----OnInitDialog------BEG\n"));
	//初始化提示信息
	SetDlgItemTextW(EDIT_BEGIN, _T("输入查询起点"));
	SetDlgItemTextW(EDIT_END, _T("输入终点"));
	///////////////////////////////////////////////////////////////////////////////////////
	//字体
	GetDlgItem(EDIT_BEGIN)->SetFont(GetAppFont(_T("EDIT")));
	GetDlgItem(EDIT_END)->SetFont(GetAppFont(_T("EDIT")));
	//初始化数据库连接
	//首先得到数据库文件地址
	CString dbpath,dbstr=_T("");
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
	//数据库连接完毕，下面读取一些基础信息
	CString fxxx;
	//得到有多少条线路
	_variant_t vvc;
	m_pRecordset = m_pConnection->Execute(_T("SELECT LINECOUNT FROM LINECOUNT WHERE ID=3"),NULL, adCmdText);
	vvc = m_pRecordset->GetCollect(_variant_t((long)0));
	LineConut = vvc.lVal;
	fxxx.Format(_T("\t-line count: %d\n"),LineConut);
	csflog.WriteString(fxxx);
	m_pRecordset->Close();
	//数据库连接代码完毕
	//加载地图框架
	mapDlg = new CMapDlg;
	mapDlg->Create(IDD_DIALOG_MAPS, GetDlgItem(TAB_MAP_FRAME));
	GetDlgItem(TAB_MAP_FRAME);
	CRect TabRect;
	GetDlgItem(TAB_MAP_FRAME)->GetClientRect(TabRect);
	mapDlg->SetWindowPos(NULL, TabRect.left, TabRect.top + 21, TabRect.Width() - 3, TabRect.Height() - 23, SWP_SHOWWINDOW);
	//

	csflog.WriteString(_T("-----in-----[FUNCTION]----OnInitDialog------END\n\n\n"));
	csflog.Close();
	//构建地图缓存文件
	if (!CreateMapCache())
	{
		MessageBoxW(_T("无法利用数据库中的数据构建线路缓存！"), _T("错误"), MB_OK | MB_ICONERROR);
	}
	//皮肤
	SkinH_Attach();
	//设置ComboBox的数据
	for (int i = 0; i < GetLinecount(); i++)
	{
		logstr.Format(_T("%d"), i+1);
		m_combobox.AddString(logstr);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMLUDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMLUDlg::OnPaint()
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
HCURSOR CMLUDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMLUDlg::OnBnClickedLookUp()
{
	CString begname, endname;
	GetDlgItemTextW(EDIT_BEGIN, begname);
	GetDlgItemTextW(EDIT_END, endname);
	//从文件中读取起点，终点站点数据
	CFile fff;
	MMD_NODE mnb;
	fff.Open(PATH_MAP_CACHE, CFile::modeRead);
	bool begfound = false, endfound = false;
	while (fff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		if (mnb.id == -1)
		{
			fff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		CString name(mnb.name);
		if (name == begname)
		{
			SD_BEG.id = mnb.id;
			SD_BEG.line = mnb.line;
			SD_BEG.ltime = mnb.ltime;
			SD_BEG.rtime = mnb.rtime;
			SD_BEG.lprice = mnb.lprice;
			SD_BEG.rprice = mnb.rprice;
			strcpy_s(SD_BEG.name, mnb.name);
			SD_BEG.transfer = mnb.transfer;
			begfound = true;
		}
		if (name == endname)
		{
			SD_END.id = mnb.id;
			SD_END.line = mnb.line;
			SD_END.ltime = mnb.ltime;
			SD_END.rtime = mnb.rtime;
			SD_END.lprice = mnb.lprice;
			SD_END.rprice = mnb.rprice;
			strcpy_s(SD_END.name, mnb.name);
			SD_END.transfer = mnb.transfer;
			endfound = true;
		}
	}
	fff.Close();
	//如果起点或者终点有一点没查到，就报错，然后退出程序
	if (!(begfound && endfound)) 
	{
		MessageBoxW(_T("请输入正确的起点站或者是终点站！"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (!mapDlg->LookUpPath(SD_BEG, SD_END))
	{
		MessageBoxW(_T("请输入正确的起点站或者是终点站！"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	//更新相关耗散
	CString  lsd;
	lsd.Format(_T("票价：%d 元"), LOOKUP_PRICE);
	SetDlgItemTextW(IDC_MFCBUTTON_INFO_PRICE, lsd);
	lsd.Format(_T("预计耗时：%d 分钟"), LOOKUP_TIME);
	SetDlgItemTextW(IDC_MFCBUTTON_INFO_time, lsd);
	SetDlgItemTextW(STATIC_CURRENT, _T(""));
}


void CMLUDlg::OnEnSetfocusBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString raw = _T("");
	GetDlgItemTextW(EDIT_BEGIN, raw);
	if (raw == _T("输入查询起点"))
	{
		SetDlgItemTextW(EDIT_BEGIN, _T(""));
	}
	begfucused = true;
	SetDlgItemTextW(STATIC_CURRENT, _T("输入站点名，或者在地图上选择"));
}


void CMLUDlg::OnEnSetfocusEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString raw = _T("");
	GetDlgItemTextW(EDIT_END, raw);
	if (raw == _T("输入终点"))
	{
		SetDlgItemTextW(EDIT_END, _T(""));
	}
	endfocused = true;
	SetDlgItemTextW(STATIC_CURRENT, _T("输入站点名，或者在地图上选择"));
}


void CMLUDlg::OnEnKillfocusBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString raw = _T("");
	GetDlgItemTextW(EDIT_BEGIN, raw);
	if (raw == _T("") || raw == _T(" "))
	{
		SetDlgItemTextW(EDIT_BEGIN, _T("输入查询起点"));
	}
	begfucused = false;
}


void CMLUDlg::OnEnKillfocusEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString raw = _T("");
	GetDlgItemTextW(EDIT_END, raw);
	if (raw == _T("") || raw == _T(" "))
	{
		SetDlgItemTextW(EDIT_END, _T("输入终点"));
	}
	endfocused = false;
}

const _ConnectionPtr CMLUDlg::GetConnectionPtr()
{
	return m_pConnection;
}

const _RecordsetPtr CMLUDlg::GetRecordsetPtr()
{
	return m_pRecordset;
}

const bool CMLUDlg::CreateMapCache()
{
	//首先我们要得到有多少条线路(在LineCount中)
	//然后一次性读出线路数据和所有站点数据
	_RecordsetPtr LINEDATA, STATIONDATA;
	LINEDATA.CreateInstance(_T("ADODB.Recordset"));
	STATIONDATA.CreateInstance(_T("ADODB.Recordset"));
	if (!KSQL_Excute(_T("SELECT * FROM LINE_DATA"), LINEDATA))
	{
		MessageBoxW(_T("从数据库中读取线路数据出错！"), _T("错误"), MB_OK | MB_ICONERROR);
		return false;
	}
	if (!KSQL_Excute(_T("SELECT * FROM STATION_DATA"), STATIONDATA))
	{
		MessageBoxW(_T("从数据库中读取站点数据出错！"), _T("错误"), MB_OK | MB_ICONERROR);
		return false;
	}
	//最后按照平移缓存的地图格式写入文件
	MMD_HEAD mhb;
	MMD_NODE mnb,SPILT_NODE;
	_variant_t vbb;
	SPILT_NODE.id = -1;
	CFile mcf;
	mcf.Open(PATH_MAP_CACHE, CFile::modeCreate | CFile::modeWrite);
	for (int i = 0; i < LineConut; i++)
	{
		//首先要写入一个分隔符
		mcf.Write(&SPILT_NODE, sizeof(MMD_NODE));
		//然后写入线路头数据
		vbb = LINEDATA->GetCollect(_T("RGB_R"));
		mhb.RGB[0] = (int)vbb.lVal;
		vbb = LINEDATA->GetCollect(_T("RGB_G"));
		mhb.RGB[1] = (int)vbb.lVal;
		vbb = LINEDATA->GetCollect(_T("RGB_B"));
		mhb.RGB[2] = (int)vbb.lVal;
		mhb.bound = 3;
		vbb = LINEDATA->GetCollect(_T("LINEID"));
		int plid = (int)vbb.lVal;
		mcf.Write(&mhb, sizeof(MMD_HEAD));
		linecount++;
		//开始循环寻找属于当前线路的站点
		while (!STATIONDATA->adoEOF)
		{
			vbb = STATIONDATA->GetCollect(_T("LINE"));
			int lli = (int)vbb.lVal;
			//判断是否属于当前线路
			if (lli == plid)
			{
				mnb.line = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("ID"));
				mnb.id = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("STANAME"));
				USES_CONVERSION;
				char *p = W2A((LPCTSTR)vbb.lVal);
				strcpy_s(mnb.name, p);
				vbb = STATIONDATA->GetCollect(_T("LTIME"));
				mnb.ltime = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("RTIME"));
				mnb.rtime = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("LPRICE"));
				mnb.lprice = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("RPRICE"));
				mnb.rprice = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("POS_X"));
				mnb.p.x = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("POS_Y"));
				mnb.p.y = (int)vbb.lVal;
				vbb = STATIONDATA->GetCollect(_T("TRANSFER"));
				mnb.transfer = (bool)vbb.lVal;
				//写入文件
				mcf.Write(&mnb, sizeof(MMD_NODE));
			}
			STATIONDATA->MoveNext();
		}
		STATIONDATA->MoveFirst();
		LINEDATA->MoveNext();
	}
	mcf.Close();
	STATIONDATA->Close();
	LINEDATA->Close();
	return true;
}

void CMLUDlg::SetLookupPrice(const int price)
{
	LOOKUP_PRICE = price;
}

void CMLUDlg::SetLookupTime(const int time)
{
	LOOKUP_TIME = time;
}

void CMLUDlg::SetTrsstrData(const CString dta)
{
	SetDlgItemTextW(IDC_MFCBUTTON_INFO_TRANSFER, dta);
}

void CMLUDlg::SetLUbeg(const CString name)
{
	SetDlgItemText(EDIT_BEGIN, name);
}

void CMLUDlg::SetLUend(const CString name)
{
	SetDlgItemText(EDIT_END, name);
}

const bool CMLUDlg::IsBegFocused()
{
	return begfucused;
}

const bool CMLUDlg::IsEndFocused()
{
	return endfocused;
}

const int CMLUDlg::GetLinecount()
{
	return linecount;
}

const bool CMLUDlg::IsHighlightPressed()
{
	return highlightpressed;
}

void CMLUDlg::SetHighlightPressedStatus(const bool sta)
{
	highlightpressed = sta;
}

//该函数用来获取一些自定义字体
CFont * CMLUDlg::GetAppFont(const CString & fontName)
{
	CFont * f;
	f = new CFont;
	if (fontName == _T("EDIT"))
	{
		f->CreateFont(20, // nHeight 
			0, // nWidth 
			0, // nEscapement 
			0, // nOrientation 
			FW_BOLD, // nWeight 
			FALSE, // bItalic 
			FALSE, // bUnderline 
			0, // cStrikeOut 
			DEFAULT_CHARSET, // nCharSet 
			OUT_DEFAULT_PRECIS, // nOutPrecision 
			CLIP_DEFAULT_PRECIS, // nClipPrecision 
			DEFAULT_QUALITY, // nQuality 
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
			_T("Arial")); // lpszFac 
	}
	else if (fontName == _T("BORAD"))
	{
		f->CreateFont(25, // nHeight 
			0, // nWidth 
			0, // nEscapement 
			0, // nOrientation 
			FW_BOLD, // nWeight 
			TRUE, // bItalic 
			FALSE, // bUnderline 
			0, // cStrikeOut 
			ANSI_CHARSET, // nCharSet 
			OUT_DEFAULT_PRECIS, // nOutPrecision 
			CLIP_DEFAULT_PRECIS, // nClipPrecision 
			DEFAULT_QUALITY, // nQuality 
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
			_T("Arial")); // lpszFac 
	}
	else
	{
		f->CreateFont(10, 0, 0, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	}

	return f;
}


void CMLUDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	CRect TabRect;
	GetDlgItem(TAB_MAP_FRAME)->GetClientRect(TabRect);
	mapDlg->SetWindowPos(NULL, TabRect.left, TabRect.top + 21, TabRect.Width() - 3, TabRect.Height() - 23, SWP_SHOWWINDOW);
}


void CMLUDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//****第一次调用OnSize的时候控件还未被创建，故需要做以下判断
	if (GetDlgItem(TAB_MAP_FRAME) != NULL && IsWindow(GetDlgItem(TAB_MAP_FRAME)->m_hWnd))
	{
		CRect TabRect;
		CWnd * p = GetDlgItem(TAB_MAP_FRAME);
		p->GetClientRect(TabRect);
		mapDlg->SetWindowPos(NULL, TabRect.left, TabRect.top + 21, TabRect.Width() - 3, TabRect.Height() - 23, SWP_SHOWWINDOW);
	}
}

//该函数用来执行SQL语句，
const bool CMLUDlg::KSQL_Excute(const CString cmd, _RecordsetPtr & rp)
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
const bool CMLUDlg::KSQL_Excute(const CString cmd)
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

//该函数用来检查记录集中是否存在数据
//传入参数：记录集对象
//返回值：如果存在数据则返回true，否则false
const bool CMLUDlg::KSQL_CheckRecordSetGotValue(const _RecordsetPtr rp)
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

void CMLUDlg::OnBnClickedMfcbuttonInfoPrice()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMLUDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void CMLUDlg::OnBnClickedButtonClearhighlight()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_CLEARHIGHLIGHT)->EnableWindow(FALSE);
	mapDlg->ClearMapOnSceen();
	mapDlg->OnPaint();
}


void CMLUDlg::OnCbnCloseupComboHighlight()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_combobox.GetCurSel();
	if (index < 0)
	{
		return;
	}
	CString selstr;
	m_combobox.GetLBText(index, selstr);
	int lid = _ttoi(selstr);
	if (lid >= GetLinecount() || lid < 0)
	{
		return;
	}
	//下面开始高亮指定线路
	GetDlgItem(IDC_BUTTON_CLEARHIGHLIGHT)->EnableWindow(TRUE);
	mapDlg->HighlightLine(lid);

}


void CMLUDlg::OnBnClickedButtonReload()
{
	// TODO: 在此添加控件通知处理程序代码
	mapDlg->ResetMap();
}





