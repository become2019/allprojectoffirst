#pragma once
class KLog
{
public:
	KLog();
	KLog(const CString &filepath);
	~KLog();
	const bool DebugLog(const CString &data);
	const bool DebugLogA(const CString &data);
	void SetDegbugLogName(const CString &filename);
	void SetPrefix(const CString &pfs);
	void ClearPrefix();
private:
	CString logpath;
	CString prefix;
	CStdioFile csf;
	bool open = false;

};

