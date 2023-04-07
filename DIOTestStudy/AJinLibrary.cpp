// AJinLibrary.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "AJinLibrary.h"
#include "DIOTestStudyDlg.h"


// CAJinLibrary

IMPLEMENT_DYNAMIC(CAJinLibrary, CWnd)

CDIOTestStudyDlg *pMainDlgAJ;
CAJinLibrary::CAJinLibrary()
{
	pMainDlgAJ = (CDIOTestStudyDlg *)AfxGetMainWnd();
}

CAJinLibrary::~CAJinLibrary()
{
}


BEGIN_MESSAGE_MAP(CAJinLibrary, CWnd)
END_MESSAGE_MAP()
BOOL CAJinLibrary::LibraryOpen()
{
	DWORD Code = AxlOpen(7);
	if (Code == AXT_RT_SUCCESS)
		return TRUE;
	else
	{
		return FALSE;
	}
}

BOOL CAJinLibrary::LibraryClose()
{
	if (AxlIsOpened())
	{
		AxlClose();
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}

}
BOOL CAJinLibrary::IsLibraryOpen()
{
	if (AxlIsOpened())
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}

}

long CAJinLibrary::GetModuleCount()
{
	long IModuleCounts;

	AxdInfoGetModuleCount(&IModuleCounts);

	return IModuleCounts;
}
void CAJinLibrary::IsGetModuleCount()
{
	long IModuleCounts;
	DWORD Code = AxdInfoGetModuleCount(&IModuleCounts);

	char* ModuleCount = new char[200];
	if (Code == AXT_RT_SUCCESS)
	{
		TRACE(_T("Number of module: %d\n"), IModuleCounts);
		sprintf_s(ModuleCount, 200, "Number of module: %d\n", IModuleCounts);
		TCHAR szUniCode[200] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ModuleCount, strlen(ModuleCount), szUniCode, 200);
	}
	else
	{
		TRACE(_T("AxdInfoGetModuleCount() : ERROR code Ox%x\n"), Code);
		sprintf_s(ModuleCount, 200, "AxdInfoGetModuleCount() : ERROR code Ox%x\n", Code);
		TCHAR szUniCode[200] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ModuleCount, strlen(ModuleCount), szUniCode, 200);
	}
}

BOOL CAJinLibrary::GetModuleInfo(int ModuleNo)
{
	if (AxdInfoGetModule(ModuleNo, NULL, NULL, NULL) == AXT_RT_SUCCESS)
		return TRUE;
	else
	{
		return FALSE;
	}
}

CString CAJinLibrary::StrCurrentTime()
{
	CString CStrCurrentTime;
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);

	CStrCurrentTime.Format(_T("%02d:%02d:%02d:%03d"),
		CurrentTime.wHour,
		CurrentTime.wMinute,
		CurrentTime.wSecond,
		CurrentTime.wMilliseconds);

	return CStrCurrentTime;
}

SYSTEMTIME CAJinLibrary::TCurrentTime()
{
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);

	return CurrentTime;
}


void CAJinLibrary::SaveActionLogFile(CString str)
{
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	CString filePath, directoryPath;
	CStdioFile File;
	BOOL bOpen;
	CString CStrCurTime;
	setlocale(LC_ALL, "Korean");
	directoryPath.Format(_T("../Log\\%4d년"), CurrentTime.wYear);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월"), CurrentTime.wYear, CurrentTime.wMonth);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	CreateDirectory(directoryPath, NULL);

	filePath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\ActionLog.txt"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	bOpen = File.Open(filePath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite);
	if (!bOpen)
	{
		AfxMessageBox(_T("LOG File Create/Open Error"));
	}
	else
	{
		CStrCurTime = StrCurrentTime();
		File.SeekToEnd();
		File.WriteString(CStrCurTime);
		File.WriteString(_T("| "));
		File.WriteString(str);
		File.WriteString(_T("\n"));
		File.Close();
	}
	pMainDlgAJ->pLogDlg->AddActionLog(str, CStrCurTime);
}

//Compare Time = 시작시간, Target Time = 종료시간
int CAJinLibrary::CompareSystemTime(SYSTEMTIME stTargetTime, SYSTEMTIME stCompareTime)
{
	int TargetTime = stTargetTime.wHour * 3600000 + stTargetTime.wMinute * 60000 + stTargetTime.wSecond * 1000 + stTargetTime.wMilliseconds;

	int CompareTime = stCompareTime.wHour * 3600000 + stCompareTime.wMinute * 60000 + stCompareTime.wSecond * 1000 + stCompareTime.wMilliseconds;

	if (TargetTime > TargetTime || TargetTime < 0 || CompareTime < 0)
		return -1;
	else
	{
		return TargetTime - CompareTime;
	}
}

CString  CAJinLibrary::GetStrHowLong(int nHowLong, CString str)
{
	CString strFin;

	if (nHowLong < 1000)
	{
		strFin.Format(_T("\n%s 소요시간| %2d%2d%2d%3d"), str, 0, 0, 0, nHowLong);
	}
	else if (60000 > nHowLong &&  nHowLong >= 1000) //1초 넘어감
	{
		int nSec = nHowLong / 1000;
		int nMS = nHowLong % 1000;
		strFin.Format(_T("\n%s 소요시간| %2d%2d%2d%3d"), str, 0, 0, nSec, nMS);
	}
	else if (3600000 > nHowLong && nHowLong>= 60000) //1분 넘어감
	{
		int nSec = nHowLong / 1000;
		int nMin = nSec / 60;
		nSec = nSec % 60;
		int nMS = nHowLong % 1000;
		strFin.Format(_T("\n%s 소요시간| %2d%2d%2d%3d"), str, 0, nMin, nSec, nMS);
	}
	else //1시간 넘어감
	{
		int nSec = nHowLong / 1000;
		int nHour = nSec / 3600;
		int nMin = (nSec % 3600) / 60;
		nSec = nSec % 60;
		int nMS = nMin % 1000;
		strFin.Format(_T("\n%s 소요시간| %2d%2d%2d%3d"), str, nHour, nMin, nSec, nMS);
	}
	return strFin;
}
// CAJinLibrary 메시지 처리기입니다.


