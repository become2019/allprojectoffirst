#pragma once


 struct MMD_HEAD {
	short RGB[3];       //地图线路的颜色
	short bound;		  //线路绘制粗细
};
 struct MMD_NODE {
	CPoint p;  		 //线路中每个点的位置
	int id;			//如果該点为站台，则这里会有编号，否则为0
	char name[30];   //储存站点名
	int line = 0;     //储存父线路
	int ltime = 0;   //储存时间
	int rtime = 0;
	int lprice = 0;   //储存票价
	int rprice = 0;
	bool transfer = false;  //标记是否是换乘站点
};
// CMDDlg 对话框

class CMDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMDDlg)

public:
	CMDDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMDDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAP_DRAWER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	const bool OnOpenExistFile(const CString filename, const bool blueprint = false);
	const void Undo();
private:
	CPoint lbeg;
	CPoint lend;
	int station_id = 0;
	CString LINEIDBUF = _T("");
	int lineid = 0;
	bool RunOnceFuncTimer = true;
	bool FIRST_TIME_L_BUTTON_DOWN = true;
	//下面2个变量储存了一条地铁线路的起点站与终点站的位置
	//这2个变量会在第一次遇到WM_LBUTTONUP消息的时候被写入文件
	//数据库更新会发生在第一次相应WM_LBUTTONDOWN与WM_LBUTTONUP的时候
	MMD_NODE STA_BEG;

	MMD_HEAD MAP_HEAD;
    MMD_NODE TAG_METRO_LINE_SPILT;
	//下面两个变量定义了判断范围
	const int RANGE_X = 111;
	const int RANGE_Y = 111;
	bool CheckStationVaild(const CPoint p,int *offbyte=nullptr);//该函数用来检查添加的站点是否合法
	const bool SetStationANDWrite(const CPoint p,const int stationid,const int offbyte=0); //该函数用来获得离用户单击点最近的CPoint
	const bool LoadMaps(const CString & path, const bool load_unfinished=true, const bool load_bule_print=false);
	const bool KSQL_Excute(const CString cmd);
	const bool KSQL_Excute(const CString cmd, _RecordsetPtr & rp);
	const bool KSQL_CheckRecordSetGotValue(const _RecordsetPtr rp);
	//执行一次，以便从父窗口获得关键数据
	void RunOnceFunction();
	CStdioFile csflog;
	CString logstr;
	const CString STRING_BUFFER_FILENEMA = _T("line.cache");
	//数据库相关变量
	_RecordsetPtr m_RecordsetPtr;
};
