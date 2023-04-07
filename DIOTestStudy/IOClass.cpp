// IOClass.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "IOClass.h"
#include "AJinLibrary.h"
#include "DIOTestStudyDlg.h"

// CIOClass

IMPLEMENT_DYNAMIC(CIOClass, CWnd)
CDIOTestStudyDlg *pDIOTestIOClass;
CAJinLibrary *pAJIOClass;
CIOClass::CIOClass()
{
	pDIOTestIOClass = (CDIOTestStudyDlg *)AfxGetMainWnd();
	pAJIOClass = new CAJinLibrary;
}

CIOClass::~CIOClass()
{
}


BEGIN_MESSAGE_MAP(CIOClass, CWnd)
END_MESSAGE_MAP()

BOOL CIOClass::GetInput(int ModuleNo, int nPort)
{
	DWORD dwTemp;

	AxdiReadInportBit(ModuleNo, nPort, &dwTemp);
	if (TRUE == dwTemp)
		return TRUE;
	return FALSE;
}
BOOL CIOClass::GetOutput(int ModuleNo, int nPort)
{
	DWORD dwTemp;

	AxdoReadOutportBit(ModuleNo, nPort, &dwTemp);
	if (TRUE == dwTemp)
		return TRUE;
	return FALSE;
}

void CIOClass::SetOutput(int ModuleNo, int nPort, BOOL b)
{
	AxdoWriteOutportBit(ModuleNo, nPort, b);
}

long CIOClass::GetInputCount(int ModuleNo)
{
	long IInputCounts;
	AxdInfoGetInputCount(ModuleNo, &IInputCounts);    //Input 접점 갯수
	if (IInputCounts < 0)
		IInputCounts = 16;
	return IInputCounts;
}
long CIOClass::GetOutputCount(int ModuleNo)
{
	long IOutputCounts;
	AxdInfoGetOutputCount(ModuleNo, &IOutputCounts);  //Output 접점 갯수
	if (IOutputCounts < 0)
		IOutputCounts = 16;
	return IOutputCounts;
}

BOOL CIOClass::ISDioModuleExist()
{
	DWORD dwStatus;
	AxdInfoIsDIOModule(&dwStatus);
	if (dwStatus == STATUS_EXIST)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// CIOClass 메시지 처리기입니다.


