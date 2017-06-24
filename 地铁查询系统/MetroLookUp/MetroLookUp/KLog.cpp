#include "KLog.h"


KLog::KLog()
{
}

KLog::KLog(const CString & filepath)
{
	logpath = filepath;
}


KLog::~KLog()
{
}

const bool KLog::DebugLog(const CString & data)
{
	const CString CLRF = _T("\n");
	if (!open)
	{
		csf.Open(logpath, CFile::modeWrite | CFile::modeCreate);
		open = true;
	}
	else
	{
		csf.Open(logpath, CFile::modeWrite | CFile::modeCreate|CFile::modeNoTruncate);		
	}
	csf.SeekToEnd();
	if (prefix != _T(""))
	{
		csf.WriteString(prefix + _T(":"));
	}
	csf.WriteString(data);
	csf.WriteString(CLRF);
	csf.Close();
	return true;
}

const bool KLog::DebugLogA(const CString & data)
{
	const CString CLRF = _T("\n");
	csf.Open(logpath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	csf.SeekToEnd();
	if (prefix != _T(""))
	{
		csf.WriteString(prefix + _T(":"));
	}
	csf.WriteString(data);
	csf.WriteString(CLRF);
	csf.Close();
	return true;
}

void KLog::SetDegbugLogName(const CString & filename)
{
	logpath = filename;
}

void KLog::SetPrefix(const CString & pfs)
{
	prefix = pfs;
}

void KLog::ClearPrefix()
{
	prefix = _T("");
}
