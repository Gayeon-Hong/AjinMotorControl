// MonitorClass.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "MonitorClass.h"


// CMonitorClass

IMPLEMENT_DYNAMIC(CMonitorClass, CWnd)

CMonitorClass::CMonitorClass()
{

}

CMonitorClass::~CMonitorClass()
{
}


BEGIN_MESSAGE_MAP(CMonitorClass, CWnd)
END_MESSAGE_MAP()

double CMonitorClass::GetCmdPos(long m_lAxisNo)
{
	double dCmdPos;
	AxmStatusGetCmdPos(m_lAxisNo, &dCmdPos);
	return dCmdPos;
}

double CMonitorClass::GetActPos(long m_lAxisNo)
{
	double dActPos;
	AxmStatusGetActPos(m_lAxisNo, &dActPos);
	return dActPos;
}

double CMonitorClass::GetPosErr(long m_lAxisNo)
{
	double dPosErr;
	AxmStatusReadPosError(m_lAxisNo, &dPosErr);
	return dPosErr;
}


// CMonitorClass 메시지 처리기입니다.


