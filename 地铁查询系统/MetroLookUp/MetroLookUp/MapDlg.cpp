// MapDlg.cpp : 实现文件
//大一下-工程实践-1-成都地铁查询系统-用户端
//张龙 2015051152 计科154
//mailto://kanchisme@gmail.com
//http://akakanch.com
//

#include "stdafx.h"
#include "MetroLookUp.h"
#include "MetroLookUpDlg.h"
#include "MapDlg.h"
#include "afxdialogex.h"

// CMapDlg 对话框

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAPS, pParent)
{
	csflog.Open(_T("log.txt"), CFile::modeWrite | CFile::modeCreate);
	csflog.Close();
	xfx.Open(_T("offset.mmld"), CFile::modeCreate | CFile::modeWrite);
	xfx.Close();
	TAG_METRO_LINE_SPILT.id = -1;
}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMapDlg 消息处理程序


void CMapDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect;
	//设置对话框背景颜色为白色
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	//加载地图
	//首先要判断平移缓存文件大小，如果很大的话，直接从平移缓存加载
	if (firstrun)
	{
		if (!LoadMaps(PATH_MAP_CACHE, false))
		{
			MessageBox(_T("获取地图数据失败！\t\n\n请检查网络连接或程序地图列表完整性。"), _T("错误"), MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		CFile fff;
		fff.Open(PATH_MOVE_MAP_CACHE, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate);
		if (fff.GetLength() < 5)
		{
			fff.Close();
			if (!LoadMaps(PATH_MAP_CACHE, false))
			{
				MessageBox(_T("获取地图数据失败！\t\n\n请检查网络连接或程序地图列表完整性。"), _T("错误"), MB_OK | MB_ICONERROR);
			}
		}
		else
		{
			fff.Close();
			if (!LoadMaps(PATH_MOVE_MAP_CACHE, false))
			{
				MessageBox(_T("获取地图数据失败！\t\n\n请检查网络连接或程序地图列表完整性。"), _T("错误"), MB_OK | MB_ICONERROR);
			}
		}
	}

	CDialog::OnPaint();
}


/*//////////查询逻辑：
1.首先根据站点名字查询到站点对应的ID，与所属线路。
2.判断2个站点是否在同一条线路上。如果是，就直接计算ID差值，然后计算耗散数据
3.如果否，查询起始站点到目标站点的，路径，需要获得经过的线路和换乘站点，然后计算耗散
4.在地图上绘制出行进线路
/*/
bool CMapDlg::LookUpPath(const STATION_DATA SD_BEG, const STATION_DATA SD_END)
{
	CString SQLstr, buf;
	CClientDC dcd(this);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 0, 0));
	dcd.SelectObject(&brush);
	CMLUDlg *mludlg = (CMLUDlg*)GetParent()->GetParent();
	//首先判断2个站点是否在同一条线路上
	if (SD_BEG.line == SD_END.line) //在同一条线路
	{
		int line=SD_BEG.line;
		//如果上面没有发生错误的话，接下来我们要计算SD_BEG到SD_END的耗散
		//然后，在地图上用红线表示出路径
		//先将地图复位
		CMapDlg::ClearMapOnSceen();
		CMapDlg::OnPaint();
		//首先，我们先在地图上划出路径
		CFile ff;
		MMD_NODE mnb;
		ff.Open(PATH_MOVE_MAP_CACHE, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate);
		if (ff.GetLength() < 5)
		{
			ff.Close();
			ff.Open(PATH_MAP_CACHE, CFile::modeRead);
		}
		//首先循环寻找起点
		int begoft = 0,endoft=0; //记录偏移量
		int oftbuf = 0;
		int i = 0;
		while (ff.Read(&mnb, sizeof(MMD_NODE)) > 0)
		{
			if (mnb.id == TAG_METRO_LINE_SPILT.id)
			{
				ff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);//跳过头部
				oftbuf += sizeof(MMD_NODE);
				oftbuf += sizeof(MMD_HEAD);
				continue;
			}
			if (mnb.id == SD_BEG.id)
			{
				begoft = oftbuf;
			}
			if (mnb.id == SD_END.id)
			{
				endoft = oftbuf;
			}
			oftbuf += sizeof(MMD_NODE);
		}
		//判断偏移量大小，因为我们要从小的偏移量开始绘图
		int id = SD_END.id;
		if (begoft > endoft)
		{
			int t = begoft;
			begoft = endoft;
			endoft = t;
			id = SD_BEG.id;
		}
		//开始绘制红线路径到地图上
		ff.SeekToBegin();
		ff.Seek(begoft, CFile::SeekPosition::current);
		MMD_NODE point;
		CPen pen(PS_SOLID, 6, RGB(255,0,0));  
		dcd.SelectObject(&pen);
		//我们要将查询路径，放入新文件中
		CFile lkp;
		lkp.Open(PATH_LOOKEDUP_CACHE, CFile::modeCreate | CFile::modeWrite);
		//下面开始绘制路径并同时计算耗散
		int price = 0, time = 0;

		ff.Read(&mnb, sizeof(MMD_NODE));
		dcd.MoveTo(mnb.p);
		dcd.Ellipse(mnb.p.x - 10, mnb.p.y - 10, mnb.p.x + 10, mnb.p.y + 10);	
		dcd.TextOutW(mnb.p.x - 111, mnb.p.y -8, _T("从这里出发->"));
		lkp.Write(&mnb, sizeof(MMD_NODE));
		while (ff.Read(&point, sizeof(MMD_NODE)) > 0)
		{
			
			dcd.LineTo(point.p);
			if (point.id == id)
			{
				break;
			}
			dcd.MoveTo(point.p);
			time += point.ltime;
			lkp.Write(&point, sizeof(MMD_NODE));
		}
		lkp.Close();
		mnb = point;
		dcd.Ellipse(mnb.p.x - 10, mnb.p.y - 10, mnb.p.x + 10, mnb.p.y + 10);
		dcd.TextOutW(mnb.p.x - 100, mnb.p.y - 8, _T("到达这里->"));
		//其次，计算耗散，然后更新数据
		price = point.lprice - mnb.lprice;
		mludlg->SetLookupPrice(price);
		mludlg->SetLookupTime(time);

		ff.Close();
	}
	else  //不在同一条线路上
	{
		//查找思路，从起始点开始，读取换成节点，然后一步一步向下展开节点
		CString lb, le, pdline=_T("");  //pdline用来储存已经处理过的站点
		lb.Format(_T("%d"), SD_BEG.line);
		le.Format(_T("%d"), SD_END.line);
		CString path;
		//然后开始进行路径寻找
		path = FindPathXX(SD_BEG.id,SD_END.id);
		//AfxMessageBox(path);
		//找到路径，下面开始绘制路径到地图上
		// @1，2，3，4@5，6，7
		//上面为处理的字符串结构，每次遇到@的时候，我们就把画笔移动到@后面的第一个ID位置再使用lineto
		//如果@后面只有一个id，则忽略这个@块
		//然后，在地图上用红线表示出路径
		//先将地图复位
		CMapDlg::ClearMapOnSceen();
		CMapDlg::OnPaint();

		int price = 0, time = 0;
		CString TRSSTR=_T("换乘提示：\n"), bufstr;
		MMD_NODE mnb;
		CFile fff;
		fff.Open(PATH_MOVE_MAP_CACHE, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate);
		if (fff.GetLength() < 5)
		{
			fff.Close();
			fff.Open(PATH_MAP_CACHE, CFile::modeRead);
		}
		CPen pen(PS_SOLID, 6, RGB(255, 0, 0));
		bool firststa=true;
		int i = 0;
		bool fsd = true;
		CPoint lp;
		while (true)
		{
			int nextlinebeg = path.Find(_T("@"),1);//跳过第一个@直接寻找下一个@，如果没有，下面那个则处理到字符串结束
			int firstcom = path.Find(_T(","));
			bool multiwaydraw = true;
			CString staid = path.Mid(1, firstcom -1);
			mnb = GetStation(_ttoi(staid), fff);
			CPoint linebegpoint = mnb.p;
			dcd.MoveTo(linebegpoint);
			if (fsd)
			{
				fsd = false;
				dcd.Ellipse(mnb.p.x - 10, mnb.p.y - 10, mnb.p.x + 10, mnb.p.y + 10);
				dcd.TextOutW(mnb.p.x - 111, mnb.p.y - 8, _T("从这里出发->"));
			}
			int secondcom = path.Find(_T(","), firstcom+1);  //找到第二个,
			if (secondcom > nextlinebeg && nextlinebeg != -1)//如果第二,的位置大于下条线路的@说明为换乘站点，我们就不处理，数据结构类似于这样 @1,@123,2,
			{
				multiwaydraw = false;
			}
			if (nextlinebeg != -1)
			{
				path = path.Right(path.GetLength() - firstcom - 1);  //删掉刚刚处理过的字符串（即‘@’和第一个站点‘id’和第一个‘,’）
			}
			else
			{
				path = path.Right(path.GetLength() - 1);
			}
			int firstid = mnb.id;
			CString lastid;
			if (!firststa)
			{
				i++;
				buf.Format(_T("%d:在【%s】换乘【%d】号线\n"), i,CString(mnb.name), mnb.line);
				TRSSTR += buf;
			}
			while (multiwaydraw)
			{
				staid = _T("");
				firststa = false;
				for (int i = 0; i <= path.GetLength(); i++)
				{
					if (i == path.GetLength())
					{
						MMD_NODE endmnb = GetStation(_ttoi(lastid), fff);
						mnb = GetStation(firstid, fff);
						if (firstid < _ttoi(lastid))
						{
							price += (endmnb.lprice - mnb.lprice);
						}
						else
						{
							price += (endmnb.rprice - mnb.rprice);
						}
						multiwaydraw = false;
						path = _T("");
						break;
					}
					if (CString(path[i]) == CString(_T("@")))
					{
						MMD_NODE endmnb = GetStation(_ttoi(lastid), fff);
						mnb = GetStation(firstid, fff);
						if (firstid < _ttoi(lastid))
						{
							price += (endmnb.lprice - mnb.lprice);
						}
						else
						{
							price += (endmnb.rprice - mnb.rprice);
						}
						multiwaydraw = false;
						path = path.Right(path.GetLength() - i);
						break;
					}
					if (CString(path[i]) != CString(_T(",")))
					{
						staid += path[i];
					}
					else
					{
						mnb = GetStation(_ttoi(staid), fff);
						if (mnb.id > firstid)  //计算耗时
						{
							time += mnb.ltime;
						}
						else
						{
							time += mnb.rtime;
						}
						dcd.SelectObject(&pen);
						linebegpoint = mnb.p;
						dcd.LineTo(linebegpoint);
						dcd.MoveTo(linebegpoint);
						lastid = staid;
						staid = _T("");
						lp = mnb.p;
					}
				}
			}
			if (path.GetLength()==0)
			{
				break;
			}
		}
		fff.Close();
		dcd.Ellipse(lp.x - 10, lp.y - 10, lp.x + 10, lp.y + 10);
		dcd.TextOutW(lp.x - 100, lp.y - 8, _T("到达这里->"));
		//设置价格
		mludlg->SetLookupPrice(price);
		mludlg->SetLookupTime(time);
		mludlg->SetTrsstrData(TRSSTR);
	}

	return true;
}

//该函数用来高亮一条线路，首先调用OnPaint，然后再高亮
const bool CMapDlg::HighlightLine(const int linenum)
{
	ClearMapOnSceen();
	OnPaint();
	CFile ff;
	MMD_NODE mnb;
	ff.Open(PATH_MOVE_MAP_CACHE, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate);
	if (ff.GetLength() < 5)
	{
		ff.Close();
		ff.Open(PATH_MAP_CACHE, CFile::modeRead);
	}
	bool first = true;
	CClientDC dcd(this);
	CPen pen(PS_SOLID, 7, RGB(60, 255, 32));
	CBrush brush;
	brush.CreateSolidBrush(RGB(60, 255, 32));
	dcd.SelectObject(&pen);
	dcd.SelectObject(&brush);
	while (ff.Read(&mnb, sizeof(MMD_NODE)))
	{
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			if (!first)
			{
				break;
			}
			ff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		if (first && mnb.line == linenum)
		{
			first = false;
			dcd.MoveTo(mnb.p);
			dcd.Ellipse(mnb.p.x - 7, mnb.p.y - 7, mnb.p.x + 7, mnb.p.y + 7);
			continue;
		}
		if (mnb.line == linenum)
		{
			dcd.LineTo(mnb.p);
			dcd.MoveTo(mnb.p);
			dcd.Ellipse(mnb.p.x - 7, mnb.p.y - 7, mnb.p.x + 7, mnb.p.y + 7);
		}
	}
	
	return true;
}

//加载地图的函数
//传入参数：地图文件路径，是否是查询后的地图加载
//当第二个参数为true的时候，我们需要在PATH_LOOKEDUP_CACHE中读取查询线路的绘制
//关于换乘站点的绘制方法：当绘制的为换乘站点的时候，我们需要判断是否为该站点的首次绘制，如果是，我们就绘制，否则不绘制
const bool CMapDlg::LoadMaps(const CString & mapList, const bool not_looked_up)
{

	CPoint point,lbeg;
	CString linePath;
	CClientDC dcd(this);
	CStdioFile mf;   //这个用来读取地图列表
	CFile f;		//这个用来读取每条线路
	CMLUDlg *mludlg = (CMLUDlg*)GetParent()->GetParent();

	//开始绘制地图
		f.Open(mapList, CFile::modeRead);
		MMD_HEAD mhb;
		MMD_NODE mnb;
		int ii = 0;
		while (f.Read(&mnb, sizeof(MMD_NODE)) > 0)
		{
			CPen pen;
			//遇到分隔符
			if (mnb.id == TAG_METRO_LINE_SPILT.id)
			{
				//遇到分隔符后说明接下来应该读取文件头
				f.Read(&mhb, sizeof(MMD_HEAD));
				//更改相应的画笔
				pen.CreatePen(PS_SOLID, 2, RGB(mhb.RGB[0], mhb.RGB[1], mhb.RGB[2]));
				dcd.SelectObject(&pen);

				//接下来读取一个点作为起点
				f.Read(&mnb, sizeof(MMD_NODE));
				ii = 0;
			}
			point = mnb.p;
			//站点名在这里绘制
			if (mnb.id > 0) //大于0说明是站点
			{
				CString transfer(mnb.name);
				//判断是否为换乘站点
					//绘制实心站点
					CBrush brush;
					brush.CreateSolidBrush(RGB(mhb.RGB[0], mhb.RGB[1], mhb.RGB[2]));
					dcd.SelectObject(&brush);
					dcd.Ellipse(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
					dcd.TextOutW(point.x - 30, point.y + 10, transfer);
					if (mnb.transfer)
					{
						dcd.SelectObject(&pen);
						dcd.Rectangle(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
					}
			}
			//绘制地图
			//地图绘制之前的判断
			if (!ii)
			{
				lbeg = mnb.p;
				ii = 99999;
				continue;
			}
			dcd.MoveTo(lbeg);
			dcd.LineTo(point);
			lbeg = point;
		}
		f.Close();
		if (not_looked_up)   //传入的是地图列表文件
		{

		}
	ResetTransferStationDrawedStatus();

	return true;
}



//地图平移函数
//传入参数：地图列表，方向向量
//关于分段的判断，当读到id=-1的时候，就说明新的地铁线路开始了
const bool CMapDlg::MoveMap(const CString & mapList,const CPoint &mvet)
{
	CFile mvb;
	MMD_NODE mnb;
	MMD_HEAD mhb;
	mvb.Open(PATH_MOVE_MAP_CACHE, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	//判断平移缓存文件是否建立，否则我们复制PATH_MAP_CACHE的数据到平移缓存文件
	if (mvb.GetLength() < 5)
	{
		CFile mpb;
		mpb.Open(PATH_MAP_CACHE, CFile::modeRead);
		while (mpb.Read(&mnb, sizeof(MMD_NODE)) > 0)
		{
			//判断是否为分隔符
			if (mnb.id == TAG_METRO_LINE_SPILT.id)
			{
				//复制分隔符
				mvb.Write(&mnb, sizeof(MMD_NODE));
				//复制地图头
				mpb.Read(&mhb, sizeof(MMD_HEAD));
				mvb.Write(&mhb, sizeof(MMD_HEAD));
				continue;
			}
			mvb.Write(&mnb, sizeof(MMD_NODE));
		}
		mpb.Close();
	}
	mvb.SeekToBegin();
	while (mvb.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		//判断是否为分隔符
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			mvb.Read(&mhb, sizeof(MMD_HEAD));
			continue;
		}
		mnb.p.x += (mvet.x*-1);
		mnb.p.y += (mvet.y*-1);
		int seekpos = sizeof(MMD_NODE);
		seekpos*=-1;
		mvb.Seek(seekpos, CFile::SeekPosition::current);
		mvb.Write(&mnb, sizeof(MMD_NODE));
	}
	mvb.Close();
	return true;
}
const bool CMapDlg::HaveBeenDrawed(const CString & staname)
{
	for (int i = 0; i < 30; i++)
	{
		if (DrawedTransferStation[i] == staname)
		{
			return true;
		}
	}
	return false;
}
void CMapDlg::ResetTransferStationDrawedStatus()
{
	for (int i = 0; i < 30; i++)
	{
		DrawedTransferStation[i] = _T("");
	}
}
//该函数用来获得离用户点击最近的站点，如果没有，返回站点ID为-1；
const MMD_NODE CMapDlg::GetCloseSTA(const CPoint pos,const bool no_zoom)
{
	MMD_NODE mnb;
	CFile ff;
	const int XOFT = 5;
	const int YOFT = 5;
	if (no_zoom)
	{
		ff.Open(PATH_MOVE_MAP_CACHE, CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate);
	}
	if (ff.GetLength() < 5)
	{
		ff.Close();
		ff.Open(PATH_MAP_CACHE, CFile::modeRead);
	}
	while (ff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			ff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		int xoft = mnb.p.x - pos.x;
		int yoft = mnb.p.y - pos.y;
		if (xoft < 0)
		{
			xoft *= -1;
		}
		if (yoft < 0)
		{
			yoft *= -1;
		}
		if (xoft < XOFT && yoft < YOFT)
		{
			ff.Close();
			return mnb;
		}
	}
	ff.Close();
	mnb.id = -1;
	return mnb;
}

void CMapDlg::ShowMsg(const PROCESSqueue pq,const TRS trs, const CString processed,const CString others)
{
	CStdioFile csf;
	csf.Open(_T("open_path_log.txt"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	csf.SeekToEnd();
	CString sstr;
	//sstr.Format(_T("%s:\n------------\n入栈站点id:%d\n前向读取：%d\n\n路径值：%s"), others, trs.id, (int)trs.read_ahead, trs.path);
	sstr.Format(_T("\n----------------%s:\npush_id:%d\nread_ahead:%d\npath_value:%s\nqueue size=%d\nprocess list=%s\n"), others, trs.id, (int)trs.read_ahead, trs.path,pq.size(), processed);
	csf.WriteString(sstr);
	//MessageBoxW(sstr, _T("路径展开调试信息"), MB_ICONEXCLAMATION);

	csf.Close();
}

void CMapDlg::ResetMsg()
{
	CStdioFile csf;
	csf.Open(_T("open_path_log.txt"), CFile::modeCreate | CFile::modeReadWrite);
	csf.Close();
}

//该函数是用来得到某个站点在文件中的起始位置
//未找到返回-1
const int CMapDlg::GetSTApos(const int staID,  CFile & fff)
{
	int rawpos = fff.GetPosition();
	fff.SeekToBegin();
	MMD_NODE mnb;
	while (fff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			fff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		if (mnb.id == staID)
		{
			int pos = (fff.GetPosition() - sizeof(MMD_NODE));
			fff.Seek(rawpos, CFile::SeekPosition::begin);
			return pos;
		}
	}
	fff.Seek(rawpos, CFile::SeekPosition::begin);

	return -1;
}
//该函数是用来获得地图缓存文件中指定站点所在线路的第一个站点的位置
//未找到返回-1
const int CMapDlg::GetFirstSTApos(const int staid,  CFile & fff)
{
	int rawpos = fff.GetPosition();
	fff.SeekToBegin();
	MMD_NODE mnb;
	mnb = GetStation(staid, fff);
	int line = mnb.line;
	fff.SeekToBegin();
	while (fff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			fff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		if (mnb.line == line)
		{
			int pos = (fff.GetPosition() - sizeof(MMD_NODE));
			fff.Seek(rawpos, CFile::SeekPosition::begin);
			return pos;
		}
	}
	fff.Seek(rawpos, CFile::SeekPosition::begin);

	return -1;
}
//该函数用来得到ID为trsid的换乘站点在其它线路上的id，结果需要在GV_TRSIDS数组里面找到
//返回值为除开trsid之外的所有id
const int CMapDlg::GetTRSids(const int trsid,  CFile & fff)
{
	int rawpos = fff.GetPosition();
	fff.SeekToBegin();
	MMD_NODE mnb;
	int i = 0;
	mnb = GetStation(trsid, fff);
	fff.SeekToBegin();
	CString TRSNAME(mnb.name);
	while (fff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			fff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		CString oname(mnb.name);
		if (oname == TRSNAME && mnb.id!=trsid)
		{
			GV_TRSIDS[i] = mnb.id;
			i++;
		}
	}
	fff.Seek(rawpos, CFile::SeekPosition::begin);

	return i;
}

//该函数用来根据指定id获得站点数据
const MMD_NODE CMapDlg::GetStation(const int id, CFile &fff)
{
	MMD_NODE mnb;
	fff.SeekToBegin();
	while (fff.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			fff.Seek(sizeof(MMD_HEAD), CFile::SeekPosition::current);
			continue;
		}
		if (mnb.id == id)
		{
			return mnb;
		}
	}
	return TAG_METRO_LINE_SPILT;
}

//该函数用来从beg按照read_head指定的方向读取一个站点长度的数据
//传入参数：起点，终点，读取方向，beg的path值，展开过的节点列表，待展开节点队列，文件指针
//返回值：READNODE.find指示是否找到路径，如果READNODE.find为真，则路径储存在READNODE.path中
const READNODE CMapDlg::ReadNode(const int beg, const int target,const bool read_ahead, const CString bpath, CString &processed,PROCESSqueue & po, CFile & fff)
{
	READNODE rnd;
	MMD_NODE BEG,mnb;
	int seekoft = sizeof(MMD_NODE);//回溯长度
	seekoft *= -1;
	BEG = GetStation(beg, fff);
	CString TRSIDSTR;
	TRS buf;
	if (BEG.transfer)
	{
		int trssum = GetTRSids(beg, fff);  //首先得到该换乘站点的所有id集合，储存在GV_TRSIDS集合中
		//接下来我们展开其所有ID之下的相邻站点
		//首先要处理的是与该换乘站点在同一条线路上的下一个未展开站点
		fff.Seek(GetSTApos(beg, fff), CFile::SeekPosition::begin);
		if (read_ahead)
		{
			fff.Seek(seekoft, CFile::SeekPosition::current);
		}
		else
		{
			fff.Seek(sizeof(MMD_NODE), CFile::SeekPosition::current);
		}
		fff.Read(&mnb, sizeof(MMD_NODE));
		TRSIDSTR.Format(_T("%d"), mnb.id);
		if (processed.Find(_T(",") + TRSIDSTR + _T(",")) < 0)
		{
			buf.id = mnb.id;
			buf.fpos = GetSTApos(mnb.id, fff);
			buf.path = bpath + TRSIDSTR + _T(",");
			buf.read_ahead = read_ahead;
			if (mnb.id == target)
			{
				rnd.find = true;
				rnd.path = buf.path;
				return rnd;
			}
			po.push(buf);
			TRSIDSTR.Format(_T("%d"), mnb.id);
			processed += TRSIDSTR + _T(",");
			ShowMsg(po,buf, processed,_T(">>>TRANSFER - ADD SAME LINE"));
		}
		//接下来循环处理该换乘站点的其它ID值数据
		for (int i = 0; i < trssum; i++)
		{
			int pos = GetSTApos(GV_TRSIDS[i], fff);
			int firstSTApos = GetFirstSTApos(GV_TRSIDS[i], fff);  //得到当前线路第一个站点的位置，为了避免读取出界
			fff.Seek(pos, CFile::SeekPosition::begin);
			//得到该换乘站点前面一个的站点
			if (firstSTApos != pos)  //在不是起点占的情况下我们才将它加入到下个位置
			{
				fff.Seek(seekoft, CFile::SeekPosition::current);//定位到前一个站点位置
				fff.Read(&mnb, sizeof(MMD_NODE));
				TRSIDSTR.Format(_T("%d"), mnb.id);
				if (processed.Find(_T(",") + TRSIDSTR + _T(",")) < 0)
				{
					buf.id = mnb.id;
					buf.fpos = GetSTApos(mnb.id, fff);
					TRSIDSTR.Format(_T("%d,@%d,%d,"), beg, GV_TRSIDS[i], mnb.id);
					buf.path = bpath + TRSIDSTR;
					buf.read_ahead = true;
					if (mnb.id == target)
					{
						rnd.find = true;
						rnd.path = buf.path;
						return rnd;
					}
					po.push(buf);
					ShowMsg(po,buf, processed,_T(">>>TRANSFER - ADD BEFORE STATION"));
					TRSIDSTR.Format(_T("%d"), mnb.id);
					processed += TRSIDSTR + _T(",");
				}
			}
			//得到beg后一个的站点
			fff.Seek(pos, CFile::SeekPosition::begin);
			fff.Seek(sizeof(MMD_NODE), CFile::SeekPosition::current);//定位到后一个站点位置
			fff.Read(&mnb, sizeof(MMD_NODE));
			TRSIDSTR.Format(_T("%d"), mnb.id);
			if (mnb.id != TAG_METRO_LINE_SPILT.id && processed.Find(_T(",") + TRSIDSTR + _T(",")) < 0)//在不是分隔符的情况下，才将他加入到下个位置
			{
				buf.id = mnb.id;
				buf.fpos = GetSTApos(mnb.id, fff);
				TRSIDSTR.Format(_T("%d,@%d,%d,"), beg, GV_TRSIDS[i], mnb.id);
				buf.path = bpath + TRSIDSTR;
				buf.read_ahead = false;
				if (mnb.id == target)
				{
					rnd.find = true;
					rnd.path = buf.path;
					return rnd;
				}
				po.push(buf);
				ShowMsg(po,buf, processed,_T(">>>TRANSFER - ADD BEHIND LINE"));
				TRSIDSTR.Format(_T("%d"), mnb.id);
				processed += TRSIDSTR + _T(",");
			}
		}
	}
	else
	{
		int pos = GetSTApos(beg, fff);
		int firstSTApos = GetFirstSTApos(beg, fff);  //得到当前线路第一个站点的位置，为了避免读取出界
		fff.Seek(GetSTApos(beg, fff), CFile::SeekPosition::begin);
		if (read_ahead)
		{
			if (pos == firstSTApos)
			{
				rnd.find = false;
				return rnd;
			}
			fff.Seek(seekoft, CFile::SeekPosition::current);
		}
		else
		{
			fff.Seek(sizeof(MMD_NODE), CFile::SeekPosition::current);
			if (fff.GetLength() == fff.GetPosition())//到达文件尾部
			{
				rnd.find = false;
				return rnd;
			}
		}
		fff.Read(&mnb, sizeof(MMD_NODE));
		if (mnb.id == TAG_METRO_LINE_SPILT.id)
		{
			rnd.find = false;
			return rnd;
		}
		TRSIDSTR.Format(_T("%d"), mnb.id);
		if (processed.Find(_T(",")+TRSIDSTR+_T(",")) < 0)
		{
			buf.id = mnb.id;
			buf.fpos = GetSTApos(mnb.id, fff);
			buf.path = bpath + TRSIDSTR + _T(",");
			buf.read_ahead = read_ahead;
			if (!mnb.transfer)
			{
				if (mnb.id == target)
				{
					rnd.find = true;
					rnd.path = buf.path;
					return rnd;
				}
			}
			else
			{
				//判断是否发现终点站
				int trssum = GetTRSids(mnb.id, fff);  //首先得到该换乘站点的所有id集合，储存在GV_TRSIDS集合中
				for (int r = 0; r < trssum; r++)
				{
					if (GV_TRSIDS[r] == target)
					{
						rnd.path = buf.path;
						rnd.find = true;
						return rnd;
					}
				}
			}
			po.push(buf);
			TRSIDSTR.Format(_T("%d"), mnb.id);
			processed += TRSIDSTR + _T(",");
			ShowMsg(po,buf, processed,_T(">>>NORMAL ADDING"));
		}
	}
	rnd.find = false;
	return rnd;
}

// 用该函数用来查找beg到end之间经过的最短线路集
//返回值，经过的站点ID，@分割每条线路，，分割每个站点ID
const CString CMapDlg::FindPathXX(const int beg, const int end)
{
	ResetMsg();
	CFile fff;
	fff.Open(PATH_MAP_CACHE, CFile::modeRead);
	PROCESSqueue poqueue;  //用来储存下一步要处理的站点
	CString processed = _T(""),path;
	path.Format(_T("@%d,"), beg);
	//首先读取起点站的前一个站点
	ReadNode(beg, end, true, path, processed, poqueue, fff);
	//然后读取起点站的后一个站点
	ReadNode(beg, end, false , path, processed, poqueue, fff);
	//接下来开始循环处理，直到找到终点站（理论上讲，poqueue不会为空，never）
	READNODE rnb;
	while (!poqueue.empty())
	{
		ShowMsg(poqueue, poqueue.front(), processed, _T("-------------------------------->>>queue front "));
		rnb = ReadNode(poqueue.front().id, end, poqueue.front().read_ahead, poqueue.front().path, processed, poqueue, fff);
		if (rnb.find)
		{
			fff.Close();
			return rnb.path;
		}
		poqueue.pop();
	}
	fff.Close();
	return _T("NOT FOUND");
}

void CMapDlg::ClearMapOnSceen()
{
	CClientDC dcd(this);
	CRect rect;
	GetClientRect(rect);
	dcd.FillSolidRect(rect, RGB(255, 255, 255));
}

void CMapDlg::ResetMap()
{
	ClearMapOnSceen();
	OnPaint();
}


void CMapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMLUDlg *mludlg = (CMLUDlg*)GetParent()->GetParent();
	offset = true;
	lbeg = point;
	CClientDC dcd(this);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,0,0));
	dcd.SelectObject(&brush);
	if (mludlg->IsBegFocused())
	{
		MMD_NODE mnb;
			 mnb = GetCloseSTA(point);
		if (mnb.id != -1)
		{
			mludlg->SetLUbeg(CString(mnb.name));
			dcd.Ellipse(mnb.p.x - 10, mnb.p.y - 10, mnb.p.x + 10, mnb.p.y + 10);
			dcd.TextOutW(mnb.p.x - 30, mnb.p.y + 10, _T("出发地:")+CString(mnb.name));
		}
	}
	else if (mludlg->IsEndFocused())
	{
		MMD_NODE mnb;
			mnb = GetCloseSTA(point);
		if (mnb.id != -1)
		{
			mludlg->SetLUend(CString(mnb.name));
			dcd.Ellipse(mnb.p.x - 10, mnb.p.y - 10, mnb.p.x + 10, mnb.p.y + 10);
			dcd.TextOutW(mnb.p.x - 30, mnb.p.y + 10, _T("目的地:") + CString(mnb.name));
		}
	}
	SetCursor(LoadCursor(NULL, IDC_HAND));
	

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMapDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	offset = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMapDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint mvet, draw, bbeg;
	if (offset)
	{
		//计算方向向量
		SetCursor(LoadCursor(NULL, IDC_HAND));
		mvet.x = lbeg.x - point.x;
		mvet.y = lbeg.y - point.y;
		if (!MoveMap(PATH_MAP_CACHE,mvet))
		{
			MessageBox(_T("无法拖动地图，发生未知错误！"), _T("错误"), MB_ICONERROR | MB_OK);
		}
		ClearMapOnSceen();
		LoadMaps(PATH_MOVE_MAP_CACHE,false);
		lbeg = point;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
