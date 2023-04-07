// StatusClass.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "StatusClass.h"


// CStatusClass

IMPLEMENT_DYNAMIC(CStatusClass, CWnd)

CStatusClass::CStatusClass()
{

}

CStatusClass::~CStatusClass()
{
}


BEGIN_MESSAGE_MAP(CStatusClass, CWnd)
END_MESSAGE_MAP()

void CStatusClass::SetServoOn(long m_lAxisNo)
{
	AxmSignalServoOn(m_lAxisNo, HIGH);
}

void CStatusClass::SetServoOFF(long m_lAxisNo)
{
	AxmSignalServoOn(m_lAxisNo, LOW);
}

void CStatusClass::SetAlarmOn(long m_lAxisNo)
{
	AxmSignalSetServoAlarm(m_lAxisNo, HIGH);
}

void CStatusClass::SetAlarmOFF(long m_lAxisNo)
{
	AxmSignalSetServoAlarm(m_lAxisNo, LOW);
}


DWORD CStatusClass::GetServoOnOFF(long m_lAxisNo)
{
	DWORD upStatus;
	AxmSignalIsServoOn(m_lAxisNo, &upStatus);
	return upStatus;
}

DWORD CStatusClass::GetAlarmOnOFF(long m_lAxisNo)
{
	DWORD upStatus;
	AxmSignalReadServoAlarm(m_lAxisNo, &upStatus);
	//AxmSignalGetServoAlarm(m_lAxisNo, &upStatus);
	/*if (!upStatus == UNUSED)
		AxmSignalReadServoAlarm(m_lAxisNo, &upStatus);*/
	return upStatus;
}

DWORD CStatusClass::GetInMotionHL(long m_lAxisNo)
{
	DWORD upStatus;
	AxmStatusReadInMotion(m_lAxisNo, &upStatus);
	return upStatus;
}

DWORD CStatusClass::GetInPositionIsUse(long m_lAxisNo)
{
	DWORD upUse;
	AxmSignalGetInpos(m_lAxisNo, &upUse);
	if (upUse == UNUSED)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

DWORD CStatusClass::GetInPositionHL(long m_lAxisNo)
{	
	DWORD upStatus;
	AxmSignalReadInpos(m_lAxisNo, &upStatus);
	return upStatus;
}

DWORD CStatusClass::GetHomeOnOFF(long m_lAxisNo)
{
	DWORD upStatus;
	AxmHomeReadSignal(m_lAxisNo, &upStatus);
	return upStatus;
}

DWORD CStatusClass::GetPElimitHL(long m_lAxisNo)
{
	DWORD upStatus;
	AxmSignalGetLimit(m_lAxisNo, NULL, &upStatus, NULL);
	if (upStatus == UNUSED)
		upStatus = UNUSED;
	else
	{
		AxmSignalReadLimit(m_lAxisNo, &upStatus, NULL);
	}
	return upStatus;
}

DWORD CStatusClass::GetMElimitHL(long m_lAxisNo)
{
	DWORD upStatus;
	AxmSignalGetLimit(m_lAxisNo, NULL, NULL,&upStatus);
	if (upStatus == UNUSED)
		upStatus = UNUSED;
	else
	{
		AxmSignalReadLimit(m_lAxisNo, NULL, &upStatus);
	}
	return upStatus;
}

DWORD CStatusClass::GetEMGHL(long m_lAxisNo)
{
	DWORD upStatus;
	AxmSignalGetStop(m_lAxisNo, NULL, &upStatus);
	if (upStatus == UNUSED)
		upStatus = UNUSED;
	else
	{
		AxmSignalReadStop(m_lAxisNo, &upStatus);
	}
	return upStatus;
}
DWORD CStatusClass::GetHomeSearchResult(long m_lAxisNo)
{
	DWORD upStatus;
	AxmHomeGetResult(m_lAxisNo, &upStatus);
	if (upStatus == HOME_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// CStatusClass 메시지 처리기입니다.


