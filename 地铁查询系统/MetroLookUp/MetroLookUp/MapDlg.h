//大一下-工程实践-1-成都地铁查询系统-用户端
//张龙 2015051152 计科154
//mailto://kanchisme@gmail.com
//http://akakanch.com
//
#pragma once
#include <queue>
//该结构体用来储存地图头数据
struct MMD_HEAD {		
	short RGB[3];       //地图线路的颜色
	short bound;		  //线路绘制粗细
};
//该结构体用来储存地图节点数据
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
//该结构体用来储存站点数据，主要用于缓存与路径查询
struct STATION_DATA {   
	int id = 0;			//站点ID
	char name[20];		//站点名字
	int line=0;			//站点所属线路
	int ltime = 0;		//左行耗时
	int rtime = 0;		//右行耗时
	int lprice = 0;		//左行票价
	int rprice = 0;		//右行票价
	bool transfer = false;  //标记是否是换乘站点
};
//该结构体用来储存数据库中所有换乘站点的数据
struct TRANSFER_STATION {
	int id = 0;
	bool checked = false;  //确认当前点是否已经处理了，为了在FindPathX函数中判断方便而设置
	char staname[30];		//储存换乘线路名
	char parentline[30];    //储存父线路
};
 //该结构体用来储存寻路算法中的数据
struct TRS {
	bool read_ahead = false;
	int id = 0;
	int fpos = 0;
	CString path;
};

struct READNODE {
	bool find = false;
	CString path = _T("");
};
// CMapDlg 对话框

typedef std::queue<TRS> PROCESSqueue;

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)

public:
	CMapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMapDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAPS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	//该函数用来执行路径查询与线路绘制
	bool LookUpPath(const STATION_DATA SD_BEG, const STATION_DATA SD_END);
	const bool HighlightLine(const int linenum);
	void ClearMapOnSceen();
	void ResetMap();

private:
	const bool LoadMaps(const CString & mapList,const bool not_looked_up=true);
	const bool MoveMap(const CString &mapList, const CPoint &mvet);
	const bool HaveBeenDrawed(const CString & staname);
	void ResetTransferStationDrawedStatus();
	const MMD_NODE GetCloseSTA(const CPoint pos, const bool no_zoom=true);
	void ShowMsg(const PROCESSqueue pq,const TRS trs,const CString processed,const CString others);
	void ResetMsg();

	//以下函数是用来处理在多条线路之间的路径寻找的
	const int GetSTApos(const int staID, CFile &fff);
	const int GetFirstSTApos(const int staid,  CFile &fff);
	const int GetTRSids(const int trsid,  CFile &fff);
	//
	const MMD_NODE GetStation(const int id,  CFile &fff);
	const READNODE ReadNode(const int beg, const int target, const bool read_ahead, const CString bpath, CString &processed, PROCESSqueue & po, CFile &fff);
	const CString FindPathXX(const int beg, const int end);
	/////////////
	bool offset = false;
	CPoint lbeg;
	MMD_NODE TAG_METRO_LINE_SPILT;
	CStdioFile csflog;
	CString logstr;
	TRANSFER_STATION *DATA_TRANSFER_STATION;
	const CString STR_NOT_FOUND = _T("NOT FOUND");
	const CString PATH_LOOK_LOG = _T("pathlook.log");
	const CString PATH_MAP_CACHE = _T("dbmap.cache");
	const CString PATH_MOVE_MAP_CACHE = _T("offset.mmld");
	const CString PATH_LOOKEDUP_CACHE = _T("lookup.cache");
	const CString PATH_ZOOMED_CACHE = _T("zoomed.cache");
	const CString PATH_TRANSFER_STATION_CACHE = _T("ts.cache");  //这是换乘站点的缓存文件目录
	CFile xfx;
	CString DrawedTransferStation[30] = {'\0'};
	int GV_TRSIDS[5] = { 0 };
	int drawedtsa_no = 0;
	bool firstrun = true;
	double ZOOMX = 1;
	bool zoomed = false;

	_RecordsetPtr m_RecordsetPtr;
	//地图点击相关变量
	bool begpress = false;
	bool endpress = false;
	CString begpressname;
	CString endpressname;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
