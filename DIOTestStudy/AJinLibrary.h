#pragma once



#define X 0
#define Y 1
#define Z 2
// CAJinLibrary

class CAJinLibrary : public CWnd
{
	DECLARE_DYNAMIC(CAJinLibrary)

public:
	CAJinLibrary();
	virtual ~CAJinLibrary();
	BOOL LibraryOpen();
	BOOL LibraryClose();
	BOOL IsLibraryOpen();
	long GetModuleCount();
	BOOL GetModuleInfo(int ModuleNo);
	void IsGetModuleCount();
	void SaveActionLogFile(CString str);
	CString StrCurrentTime(); 
	SYSTEMTIME TCurrentTime();
	void SaveEventLogFile();
	int CompareSystemTime(SYSTEMTIME stTargetTime, SYSTEMTIME stCompareTime);
	CString	GetStrHowLong(int nHowLong, CString str);
protected:
	DECLARE_MESSAGE_MAP()
};


