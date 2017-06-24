// MDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MetroLookup_LineDrawer.h"
#include "MDDlg.h"
#include "afxdialogex.h"
#include "MetroLookup_LineDrawerDlg.h"


// CMDDlg 对话框

IMPLEMENT_DYNAMIC(CMDDlg, CDialogEx)

CMDDlg::CMDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAP_DRAWER, pParent)
{
	TAG_METRO_LINE_SPILT.id = -1;
	csflog.Open(_T("log.txt"), CFile::modeWrite |CFile::modeCreate);
	csflog.Close();
}

CMDDlg::~CMDDlg()
{
}

void CMDDlg::DoDataExchange(CDataExchange* pDX)
{ 
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMDDlg, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMDDlg 消息处理程序


void CMDDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnLButtonUp(nFlags, point);
}

//重写添加站点的代码，为了避免站点添加得太紧密，当用户单击添加站点，并准备添加的时候，我们仅判断
//单击位置是否大概在线路上，然后将单击的位置作为新点插入文件中
//***逻辑更改：目前我们只储存站点的点数据，地铁线路改为直线线段式
void CMDDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	STA_BEG.p = point;
	CMLLDDlg * mlldlg = (CMLLDDlg*)GetParent()->GetParent();
	if (!mlldlg->IsDrawLock())
	{
		//判断当前添加的是否为起点站,如果是的话，我们首先写入地图数据
		if (mlldlg->IsAddBegStation() && !mlldlg->IsDrawLock())
		{
			//首先创建线路文件，然后写入线路头数据
			CFile f;
			f.Open(STRING_BUFFER_FILENEMA, CFile::modeCreate | CFile::modeWrite);
			MAP_HEAD = mlldlg->GetHead();
			f.Write(&MAP_HEAD, sizeof(MMD_HEAD));
			//这里先获得站点ID编号等数据
			RunOnceFunction();
			//绘制站点
			CClientDC dxc(this);
			CPen pen(PS_SOLID, MAP_HEAD.bound, RGB(MAP_HEAD.RGB[0], MAP_HEAD.RGB[1], MAP_HEAD.RGB[2]));
			dxc.SelectObject(&pen);
			dxc.Ellipse(point.x - 10, point.y - 10, point.x + 10, point.y + 10);
			dxc.TextOutW(STA_BEG.p.x - 10, STA_BEG.p.y + 5, mlldlg->GetStaBegName());
			//将站点的点的数据写入缓存文件
			STA_BEG.id = station_id;
			station_id++;
			STA_BEG.lprice = 0;
			STA_BEG.ltime = 0;
			STA_BEG.rprice = _ttoi(mlldlg->GetTermPrice());
			STA_BEG.line = mlldlg->GetLineConut()+1;
			STA_BEG.rtime = 0;
			STA_BEG.transfer = false;
			USES_CONVERSION;
			strcpy_s(STA_BEG.name, W2A(mlldlg->GetStaBegName()));
			f.Write(&STA_BEG, sizeof(MMD_NODE));
			f.Close();
			//取消添加起点的状态
			mlldlg->SetAddBegStationStatus(FALSE);
			lbeg = point;
			FIRST_TIME_L_BUTTON_DOWN = false;
		}
		//判断当前添加的是否为终点站
		if (mlldlg->IsAddEndStation())
		{
			CFile f;
			f.Open(STRING_BUFFER_FILENEMA, CFile::modeCreate | CFile::modeWrite|CFile::modeNoTruncate);
			f.SeekToEnd();
			CClientDC dxc(this);
			//更新数据库
			STA_BEG.id = station_id;
			station_id++;
			STA_BEG.p = point;
			////绘制站点，修改画笔
			CPen pen(PS_SOLID, MAP_HEAD.bound, RGB(MAP_HEAD.RGB[0], MAP_HEAD.RGB[1], MAP_HEAD.RGB[2]));
			dxc.SelectObject(&pen);
			dxc.Ellipse(point.x - 10, point.y - 10, point.x + 10, point.y + 10);
			dxc.TextOutW(STA_BEG.p.x - 10, STA_BEG.p.y + 5, mlldlg->GetStaEndName());
			//将站点的点的数据写入缓存文件
			STA_BEG.lprice = _ttoi(mlldlg->GetTermPrice());
			STA_BEG.ltime = 0;
			STA_BEG.rprice = 0;
			STA_BEG.rtime = 0;
			STA_BEG.line = mlldlg->GetLineConut() + 1;
			STA_BEG.transfer = false;
			USES_CONVERSION;
			strcpy_s(STA_BEG.name, W2A(mlldlg->GetStaEndName()));
			f.Write(&STA_BEG, sizeof(MMD_NODE));
			f.Close();
			//取消添加终点的状态
			mlldlg->SetAddEndStationStatus(FALSE);
			//终点站已经添加后，就不再允许修改
			mlldlg->SetAddStationStatus(FALSE);
			mlldlg->LockAll();
			dxc.MoveTo(lbeg);
			lbeg = point;
			dxc.LineTo(point);
		}
		if (mlldlg->IsAddStation())
		{
			CStdioFile logf;
			logf.Open(_T("dlog.txt"), CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
			logf.SeekToEnd();
			//添加站点
			STATION_DATA SD;
			CClientDC dc(this);
			CFile f;
			int offbytes = 0;
			f.Open(STRING_BUFFER_FILENEMA, CFile::modeWrite);
			f.SeekToEnd();
			//得到站点数据
			SD = mlldlg->GetStationData();
			MMD_NODE map_node;
			map_node.p = point;
			map_node.id = station_id;
			station_id++;
			//将站点的点的数据写入缓存文件
			map_node.lprice = _ttoi(SD.L_price);
			map_node.ltime = _ttoi(SD.L_time);
			map_node.rprice = _ttoi(SD.R_price);
			map_node.rtime =_ttoi(SD.R_time);
			map_node.line = mlldlg->GetLineConut() + 1;
			if (mlldlg->IsTransferStation())
			{
				map_node.transfer = true;
				mlldlg->SetTransferStationStatus(false);
				lbeg = point;
				FIRST_TIME_L_BUTTON_DOWN = false;
			}
			else
			{
				map_node.transfer = false;
			}
			USES_CONVERSION;
			strcpy_s(map_node.name, W2A(SD.name));
			f.Write(&map_node, sizeof(MMD_NODE));
			f.Close();
			//设置画笔
			CPen pen(PS_SOLID, MAP_HEAD.bound, RGB(MAP_HEAD.RGB[0], MAP_HEAD.RGB[1], MAP_HEAD.RGB[2]));
			dc.SelectObject(&pen);
			//将其在地图上绘制
			dc.Ellipse(point.x - 10, point.y - 10, point.x + 10, point.y + 10);
			dc.TextOutW(map_node.p.x - 10, map_node.p.y + 5, SD.name);
			//如果添加成功就取消添加站点的状态
			mlldlg->SetAddStationStatus(false);
			logf.Close();
			dc.MoveTo(lbeg);
			lbeg = point;
			dc.LineTo(point);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMDDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMLLDDlg *mlldlg = (CMLLDDlg*)GetParent()->GetParent();
	if (mlldlg->IsAddStation() || mlldlg->IsAddBegStation() || mlldlg->IsAddEndStation())
	{
		SetCursor(LoadCursor(NULL, IDC_CROSS));
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

const bool CMDDlg::OnOpenExistFile(const CString filename, const bool blueprint)
{
	if (!blueprint)
	{
		if (!LoadMaps(filename, false))
		{
			MessageBox(_T("加载文件出错"), _T("错误"), MB_ICONERROR | MB_OK);
			return false;
		}
	}
	else
	{
		if (!LoadMaps(filename, false,true))
		{
			MessageBox(_T("加载文件出错"), _T("错误"), MB_ICONERROR | MB_OK);
			return false;
		}
	}
	return true;
}

const void CMDDlg::Undo()
{
	CFile f;
	int offbytes = sizeof(MMD_NODE);
	offbytes *= -1;
	f.Open(STRING_BUFFER_FILENEMA, CFile::modeReadWrite);
	f.SeekToEnd();
	if(f.GetPosition() <= sizeof(MMD_HEAD))
	{
		MessageBoxW(_T("无法进行撤销操作"), _T("错误"), MB_ICONERROR);
		return;
	}
	f.Seek(offbytes, CFile::SeekPosition::current);
	MMD_NODE mnb,bkb;
	f.Read(&mnb, sizeof(MMD_NODE));
	f.Seek(offbytes*2, CFile::SeekPosition::current);
	f.Read(&bkb, sizeof(MMD_NODE));
	f.SetLength(f.GetPosition());
	f.Close();
	//接下来清除屏幕上刚刚画上去那个
	CClientDC dc(this);
	COLORREF clo = dc.GetBkColor();

	//设置画笔
	CPen pen(PS_SOLID, MAP_HEAD.bound, RGB(GetRValue(clo), GetGValue(clo), GetBValue(clo)));
	dc.SelectObject(&pen);
	//将其在地图上绘制
	dc.Ellipse(mnb.p.x - 10, mnb.p.y - 10, mnb.p.x + 10, mnb.p.y + 10);
	dc.Rectangle(mnb.p.x - 10, mnb.p.y + 5, mnb.p.x+50 , mnb.p.y + 20);
	//画白线
	dc.MoveTo(mnb.p);
	dc.LineTo(bkb.p);
}

//该函数用来检查选择的站点位置是否合法
//函数的检查逻辑：判断在以其为圆心内，半径r内是否存在地图上的点(其实是个方块)
//第二个参数用来存放第一次找到目标点的偏移量，为了方便GetNearPos而设置的
bool CMDDlg::CheckStationVaild(const CPoint p, int *offbyte)
{
	CFile f;  //用来读取raw文件
	CMLLDDlg * mlldlg = (CMLLDDlg*)GetParent()->GetParent();
	if (!f.Open(mlldlg->GetFileName(), CFile::modeRead))
	{
		MessageBox(_T("站点添加失败，无法打开缓存文件\nIn function CheckStationVaild"), _T("错误"), MB_ICONERROR | MB_OK);
	}
	MMD_HEAD mhb;
	MMD_NODE mnb;
	int xin=0, yin = 0;
	int bodys = 0;  //该变量用来记录有多少个node被读取了
	//去掉头部
	f.Read(&mhb, sizeof(MMD_HEAD));
	//接下来我们循环判断是否存在点在指定范围内
	while( f.Read(&mnb, sizeof(MMD_NODE)) > 0)
	{
		xin = mnb.p.x - p.x;
		yin = mnb.p.y - p.y;
		if (xin < 0)
		{
			xin *= -1;
		}
		if (yin < 0)
		{
			yin *= -1;
		}
		if (xin <= RANGE_X  && yin <= RANGE_Y)
		{	
			*offbyte = bodys*sizeof(MMD_NODE);
			f.Close();
			return true;
		}
		bodys++;
	}

	f.Close();
	return false;
}
//在确认用户单击添加站点的位置合法后，这个函数用来获得离用户指定点最近的地图上的点，并把該点作为站点实际坐标写入
//函数逻辑：在线路文件中遍历，直到找到坐标差最小的那个（在CheckStaitonVaild通过的情况下，一定可以找到）
//更新：通过随机读取文件，来实现数据的复写，这样可以不用开缓存文件
//参数说明：第一个为用户指定的站点坐标，第二个是站点id,第3个是为了方便文件查找，设置的起始偏移量(单位：字节)
const bool CMDDlg::SetStationANDWrite(const CPoint p,const int stationid,const int offbyte)
{
	CFile f;  //一个用来读取raw文件，一个用来写入缓存文件
	CMLLDDlg * mlldlg = (CMLLDDlg*)GetParent()->GetParent();
	csflog.Open(_T("log.txt"), CFile::modeWrite | CFile::modeNoTruncate);
	csflog.SeekToEnd();
	csflog.WriteString(_T("-----[FUNCTION]----------SetStationANDWrite---------START--------\n"));
	if (!f.Open(mlldlg->GetFileName(), CFile::modeReadWrite))
	{
		MessageBox(_T("站点添加失败，无法打开缓存文件\nIn function SetStationANDWrite"), _T("错误"), MB_ICONERROR | MB_OK);
		return false;
	}
	MMD_HEAD mhb;
	MMD_NODE mnb;
	MMD_NODE nearest[10];   //该数组用来储存多个满足条件的点
	int xin = 0, yin = 0, sum = 0;
	int min = 200, minpos = 0;
	//定位指针到CheckStationVaild函数发现的第一个满足条件的地方
	f.Read(&mhb, sizeof(MMD_HEAD));
	f.Seek(offbyte, CFile::SeekPosition::current);
	logstr.Format(_T("%d bytes\nstart find-neareast-point loop:\n"), offbyte);
	csflog.WriteString(_T("file pointer seek to ") + logstr);
	//下面开始循环查找满足条件的最近点，然后写入站点数据
	while (f.Read(&mnb, sizeof(MMD_NODE)) > 0 && sum<10)
	{
		xin = mnb.p.x - p.x;
		yin = mnb.p.y - p.y;
		logstr.Format(_T(" #%d : xin=%d,yin=%d\n\t-"), sum, xin, yin);
		csflog.WriteString(_T("pocess") + logstr);
		if (xin < 0)
		{
			xin *= -1;
		}
		if (yin < 0)
		{
			yin *= -1;
		}
		if (xin > RANGE_X  || yin > RANGE_Y)
		{
			csflog.WriteString(_T("DO NOT SATISFY\n"));
			break;
		}
		//寻找最近点
		if ((xin + yin) / 2 < min)
		{
			csflog.WriteString(_T("SATISFY FOUND\n"));
			min = (xin + yin) / 2;
			minpos = sum;
		}
		//记录范围内的10个点
		nearest[sum] = mnb;
		sum++;
	}
	csflog.WriteString(_T("find-nearest-point loop end\n"));
	//复写最近点
	int seekbyte = minpos*sizeof(MMD_NODE);
	logstr.Format(_T("%d\n"), seekbyte);
	csflog.WriteString(_T("VARIBLE seekbyte=")+logstr);
	f.SeekToBegin();
	f.Read(&mhb, sizeof(MMD_HEAD));
	f.Seek(offbyte + seekbyte, CFile::SeekPosition::current);
	logstr.Format(_T("file pointer seeked %d bytes\n"), offbyte + seekbyte);
	csflog.WriteString(logstr);
	nearest[minpos].id = stationid;
	logstr.Format(_T("station id=%d @ minpos=%d\n"), stationid, minpos);
	csflog.WriteString(logstr);
	f.Write(&nearest[minpos], sizeof(MMD_NODE));

	csflog.WriteString(_T("-----[FUNCTION]----------SetStationANDWrite---------END--------\n"));
	f.Close();
	csflog.Close();
	return true;
}


//加载地图的函数
//传入参数：地图文件路径，路径类型
//当路径类型为tue的时候，表示传入的是地图列表文件，当其为false的时候，表示传入的是地图数据文件

const bool CMDDlg::LoadMaps(const CString & path, const bool load_unfinished,const bool load_bule_print)
{
	CPoint point, lbeg;
	CClientDC dcd(this);
	CFile f;		//这个用来读取每条线路
	csflog.Open(_T("log.txt"), CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	csflog.SeekToEnd();
	CMLLDDlg *mlldlg = (CMLLDDlg*)GetParent()->GetParent();
	csflog.WriteString(_T("-[FUNCTION]--------LoadMap function START------\n"));
	csflog.Flush();
	if (!load_bule_print)   //传入的是已经存在的地图文件
	{
		csflog.WriteString(_T("load map file\n"));
		csflog.Flush();
		if (!f.Open(path, CFile::modeRead))
		{
			MessageBox(_T("地图列表文件读取失败！"), _T("错误"), MB_OK | MB_ICONERROR);
			return false;
		}
		//这里的代码用来解析 MLD 文件的数据
		//解析地图头
		MMD_HEAD mmh;
		MMD_NODE mmn;
		mmn.id = 0;
		f.Read(&mmh, sizeof(MMD_HEAD));
		CPen pen(PS_SOLID, mmh.bound, RGB(mmh.RGB[0], mmh.RGB[1], mmh.RGB[2]));  //在这里定义笔的颜色,从地图数据文件中读取
		dcd.SelectObject(&pen);
		//解析地图体
		//f.Read(&mmn, sizeof(MMD_NODE));
		int i = 0;
		//lbeg = mmn.p;
		while (f.Read(&mmn, sizeof(MMD_NODE)) > 0)
		{
			point = mmn.p;
			//对于load_unfinished，该参数是用来判断是否加载的是已经完成的地图还是仅仅是地图的编辑状态中的重绘
			//而该参数的F与T仅仅决定了站点的处理方式（绘制方式）
			//如果是站点，则在数据库中查找相应ID
			if (mmn.id > 0) //大于0说明是站点
			{
				csflog.WriteString(_T("\n-#station found,start read data from date base\n"));
				csflog.Flush();
				_variant_t VSTADATA;
				CString IDBUF,SQLstr;
				IDBUF.Format(_T("%d"), mmn.id);
				SQLstr = _T("SELECT STANAME FROM STATION_DATA WHERE ID=") + IDBUF;
				m_RecordsetPtr.CreateInstance(_T("ADODB.Recordset"));
				m_RecordsetPtr =  mlldlg->GetConnectionPtr()->Execute(SQLstr.GetBuffer(0),NULL,adCmdText);
				csflog.WriteString(_T("\t#Retrive data from datebase successfully!\n"));
				csflog.Flush();
				VSTADATA = m_RecordsetPtr->GetCollect(_variant_t((long)0));
				m_RecordsetPtr->Close();
				CString STA_NAME = (LPCTSTR)VSTADATA.lVal;
				csflog.WriteString(_T("\t#now draw station: ")+STA_NAME);
				csflog.WriteString( _T("\n"));
				//现在数据库中获得相应的站点数据
				if (load_unfinished)  //如果是重绘
				{
					csflog.WriteString(_T("\t#Draw Type: unfinished=true\n"));
					csflog.Flush();
					dcd.Ellipse(point.x - 10, point.y - 10, point.x + 10, point.y + 10);
					dcd.TextOutW(point.x - 30, point.y + 10, STA_NAME);
					logstr.Format(_T("\t#Draw Completed.Station Data: STA_ID:%d----STATION NAME= "), mmn.id);
					logstr = logstr + STA_NAME ;
					csflog.WriteString(_T("\n"));
					csflog.WriteString(logstr);
					csflog.Flush();
				}
				else  //如果是加载已经完成的地图
				{
					csflog.WriteString(_T("\t#Draw Type: unfinished=false\n"));
					csflog.Flush();
					dcd.Ellipse(point.x - 10, point.y - 10, point.x + 10, point.y + 10);
					dcd.TextOutW(point.x - 30, point.y + 10, STA_NAME);
					logstr.Format(_T("\t#Draw Completed.Station Data: STA_ID:%d----STATION NAME= "), mmn.id);
					logstr = logstr + STA_NAME ;
					csflog.WriteString(_T("\n"));
					csflog.WriteString(logstr);
					csflog.Flush();
				}
			}
			//绘制地图
			if (!i)
			{
				lbeg = point;
				i = 99999;
				continue;
			}
			dcd.MoveTo(lbeg);
			dcd.LineTo(point);
			lbeg = point;
		}
		f.Close();
	}
	else  //如果传入的地址为地图底图（即包含所有其它地铁线路的底图），则我们按照当时在MEtroLook程序中加载地图平移数据缓存的方式来加载地图底图
	{
		csflog.WriteString(_T("load blue print map file\n"));
		f.Open(path, CFile::modeRead);
		MMD_HEAD mhb;
		MMD_NODE mnb;
		int i = 0;
		csflog.WriteString(_T("Ready to read MMD_NODE data to draw\n"));
		while (f.Read(&mnb, sizeof(MMD_NODE)) > 0)
		{
			CPen pen;
			csflog.WriteString(_T("D"));
			//遇到分隔符
			if (mnb.id == TAG_METRO_LINE_SPILT.id)
			{
				csflog.WriteString(_T("\nSPILT FOUND\n"));
				//遇到分隔符后说明接下来应该读取文件头
				f.Read(&mhb, sizeof(MMD_HEAD));
				logstr = _T("");
				logstr.Format(_T("HEAD COLOR RGB(%d,%d,%d)\n\n"), mhb.RGB[0], mhb.RGB[1], mhb.RGB[2]);
				csflog.WriteString(_T("\nSPILT FOUND:head---"));
				csflog.WriteString(logstr);
				//更改相应的画笔，但是粗细会变成2
				pen.CreatePen(PS_SOLID, mhb.bound-2, RGB(mhb.RGB[0], mhb.RGB[1], mhb.RGB[2]));
				dcd.SelectObject(&pen);
				//接下来读取一个点作为起点，再读取一个点作为终点
				f.Read(&mnb, sizeof(MMD_NODE));
				i = 0;
			}
			point = mnb.p;
			//站点名在这里绘制
			if (mnb.id > 0) //大于0说明是站点
			{
				logstr.Format(_T("%s\n"), CString(mnb.name));
				csflog.WriteString(_T("STATION FOUND------") + logstr);
				dcd.Ellipse(point.x - 10, point.y - 10, point.x + 10, point.y + 10);
				dcd.TextOutW(point.x - 30, point.y + 10, logstr);
			}
			if (!i)
			{
				lbeg = point;
				i = 9999;
				continue;
			}
			//绘制地图
			dcd.MoveTo(lbeg);
			dcd.LineTo(point);
			lbeg = point;
		}
		f.Close();
	}
	csflog.WriteString(_T("\n-[FUNCTION]--------LoadMap function END------\n"));
	csflog.Close();

	return true;
}

//该函数用来执行SQL语句，
const bool CMDDlg::KSQL_Excute(const CString cmd, _RecordsetPtr & rp)
{
	CMLLDDlg *cmlldlg = (CMLLDDlg*)GetParent()->GetParent();
	CString CMD = cmd;
	try
	{
		rp = cmlldlg->GetConnectionPtr()->Execute(CMD.GetBuffer(0), NULL, adCmdText);
	}
	catch (_com_error e)
	{
		MessageBoxW(_T("错误信息：") + e.Description(), _T("数据库读取错误"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}
const bool CMDDlg::KSQL_Excute(const CString cmd)
{
	CMLLDDlg *cmlldlg = (CMLLDDlg*)GetParent()->GetParent();
	CString CMD = cmd;
	try
	{
		cmlldlg->GetConnectionPtr()->Execute(CMD.GetBuffer(0), NULL, adCmdText);
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
const bool CMDDlg::KSQL_CheckRecordSetGotValue(const _RecordsetPtr rp)
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

void CMDDlg::RunOnceFunction()
{
	if (RunOnceFuncTimer)
	{
		CMLLDDlg * mlldlg = (CMLLDDlg*)GetParent()->GetParent();
		lineid = mlldlg->GetLineConut();
		lineid++;
		LINEIDBUF.Format(_T("%d"), lineid);
		station_id = mlldlg->GetLineStationCount();
		station_id++;
		RunOnceFuncTimer = false;
	}
}
