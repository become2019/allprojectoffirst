
// MetroLookUpDlg.h : 头文件
//

#pragma once
#include "MapDlg.h"
#include "afxwin.h"


// CMLUDlg 对话框
class CMLUDlg : public CDialogEx
{
// 构造
public:
	CMLUDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_METROLOOKUP_DIALOG };
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
	afx_msg void OnBnClickedLookUp();
	afx_msg void OnEnSetfocusBegin();
	afx_msg void OnEnSetfocusEnd();
	afx_msg void OnEnKillfocusBegin();
	afx_msg void OnEnKillfocusEnd();

	//数据库操作相关
	//得到连接access数据库的链接对象
	const _ConnectionPtr GetConnectionPtr();
	//得到结果集对象
	const _RecordsetPtr GetRecordsetPtr();
	//从数据库中加载地图到文件中生成地图缓存文件
	const bool CreateMapCache();
	
	//站点查询相关
	void SetLookupPrice(const int price);
	void SetLookupTime(const int time);
	void SetTrsstrData(const CString dta);
	void SetLUbeg(const CString name);
	void SetLUend(const CString name);
	//下面2个函数是用来控制在地图上选择站点的
	const bool IsBegFocused();
	const bool IsEndFocused();
	//下面这个函数是用来返回线路数目
	const int GetLinecount();
	//下面函数是站点高亮相关
	const bool IsHighlightPressed();
	void SetHighlightPressedStatus(const bool sta);
private:
	const bool KSQL_Excute(const CString cmd);
	const bool KSQL_Excute(const CString cmd, _RecordsetPtr & rp);
	const bool KSQL_CheckRecordSetGotValue(const _RecordsetPtr rp);

	CFont *GetAppFont(const CString & fontName);
	CMapDlg *mapDlg;
	STATION_DATA SD_BEG;
	STATION_DATA SD_END;
	CStdioFile csflog;
	CString logstr;
	int linecount = 0;
	//数据库操作相关
	_ConnectionPtr m_pConnection;//连接access数据库的链接对象 
	_RecordsetPtr m_pRecordset;//结果集对象
	int LineConut = 0;      //记录当前有多少条线路
	const CString PATH_MAP_CACHE = _T("dbmap.cache");
	const CString PATH_MOVE_MAP_CACHE = _T("offset.mmld");
	//站点查询相关变量
	int LOOKUP_TIME = 0;
	int LOOKUP_PRICE = 0;
	//下面用来标记是否需要启用地图上点击选择
	bool begfucused = false;
	bool endfocused = false;
	//下面来标记线路高亮相关
	bool highlightpressed = false;

public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedMfcbuttonInfoPrice();
	afx_msg void OnClose();
	CComboBox m_combobox;
	afx_msg void OnBnClickedButtonClearhighlight();
	afx_msg void OnCbnCloseupComboHighlight();
	afx_msg void OnBnClickedButtonReload();
};
