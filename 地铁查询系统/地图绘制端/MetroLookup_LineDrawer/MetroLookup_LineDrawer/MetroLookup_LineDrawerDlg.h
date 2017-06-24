
// MetroLookup_LineDrawerDlg.h : 头文件
//

#pragma once
#include "afxcolorbutton.h"
#include "MDDlg.h"
#include "afxwin.h"

struct STATION_DATA {
	CString name;
	CString L_time;
	CString R_time;
	CString L_price;
	CString R_price;
};
// CMLLDDlg 对话框
class CMLLDDlg : public CDialogEx
{
// 构造
public:
	CMLLDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_METROLOOKUP_LINEDRAWER_DIALOG };
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
	afx_msg void OnBnClickedMfcbuttonLoadMap();
	afx_msg void OnBnClickedMfcbuttonAddStation();
	afx_msg void OnBnClickedMfcbuttonNewLine();

	const MMD_HEAD GetHead();//得到预设的头数据
	const CString GetFileName(); //得到要保存的文件名
	const STATION_DATA GetStationData();//得到站点数据
	void SetSatationData(const STATION_DATA sdta); //设置站点数据
	const bool IsAddStation();
	const bool IsAddBegStation();
	const bool IsAddEndStation();
	void SetAddBegStationStatus(const bool b);
	void SetAddEndStationStatus(const bool b);
	bool SetAddStationStatus(const bool status);  //用来设置父窗口的add_station变量
	void LockAll();  //当完成终点站的绘制后，我们要锁定绘图
	void SetTransferStationStatus(const bool st);
	const bool IsTransferStation();
	bool IsDrawLock();  //判断是否出于绘画lock状态
	bool NoDrawStatus(); //得到绘制状态
	const bool KSQL_Excute(const CString cmd);
	const bool KSQL_Excute(const CString cmd, _RecordsetPtr & rp);
	const bool KSQL_CheckRecordSetGotValue(const _RecordsetPtr rp);
	CMFCColorButton m_color_button;
	//数据库操作相关
	//得到连接access数据库的链接对象
	const _ConnectionPtr GetConnectionPtr();
	//得到结果集对象
	const _RecordsetPtr GetRecordsetPtr();
	//得到某线路最后一个站点的id
	const int GetLineStationCount();
	//得到当前有多少条线路
	const int GetLineConut();

	//下面的函数用来设置【站点全局】变量
	
	//设置文件名
	void SetFileName(const CString fn);
	//设置起始站点名字
	void SetStaBegName(const CString ne);
	//设置终点站名字
	void SetStaEndName(const CString ne);
	//设置全程票价
	void SetTermPrice(const int price);
	//设置线路颜色
	void SetLineColor(const COLORREF clr);
	
	//下面的函数用来得到站点的全局变量

	//得到起始站点名字
	const CString GetStaBegName();
	//得到终点名字
	const CString GetStaEndName();
	//得到全程票价
	const CString GetTermPrice();
private:
	//这里是一些用来记录状态的变量
	bool GV_BOOL_BEG_STATION_ADDED = false;
	bool GV_BOOL_END_STATION_ADDED = false;
	const CString STRING_BUFFER_FILENEMA = _T("line.cache");
	///站点全局变量定义开始
	CString filename;
	CString STA_NAME_BEG;
	CString STA_NAME_END;
	int STA_TERM_PRICE;
	///站点全局变了定义结束
	bool add_station = false;
	bool add_station_is_transfer = false;
	STATION_DATA SD;
	CMDDlg *mddlg;
	MMD_HEAD MHE;
	bool draw_lock = false;
	bool NO_DRAW = true;
	bool HAVE_SYNC = false;
	//数据库操作相关
	_ConnectionPtr m_pConnection;//连接access数据库的链接对象 
	_RecordsetPtr m_pRecordset;//结果集对象
	int LineStationCount = 0;   //记录每条线路有多少站点，（其实是某线路最后一个站点的id）
	int LineConut = 0;      //记录当前有多少条线路
	
public:
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedMfcbuttonSaveMap();


	afx_msg void OnClose();
	CStatic m_static_linecount;
	afx_msg void OnBnClickedMfcbuttonLoadBlueprint();
	afx_msg void OnBnClickedButtonAddEnd();
	afx_msg void OnBnClickedButtonAddBegStation();
	afx_msg void OnBnClickedButtonUndo();
};
