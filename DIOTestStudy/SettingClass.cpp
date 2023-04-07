// SettingClass.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "SettingClass.h"


// CSettingClass

IMPLEMENT_DYNAMIC(CSettingClass, CWnd)

CSettingClass::CSettingClass()
{

}

CSettingClass::~CSettingClass()
{
}


BEGIN_MESSAGE_MAP(CSettingClass, CWnd)
END_MESSAGE_MAP()

void CSettingClass::SetParameter(long m_lAxisNo, double dMovePos, double dMoveVel, double dMoveAcc, double dMoveDec)
{
	AxmMotSetParaLoad(m_lAxisNo, dMovePos, dMoveVel, dMoveAcc, dMoveDec);
}

void CSettingClass::GetParameter(long m_lAxisNo, double *dMovePos, double *dMoveVel, double *dMoveAcc, double *dMoveDec)
{
	AxmMotGetParaLoad(m_lAxisNo, dMovePos, dMoveVel, dMoveAcc, dMoveDec);
}
BOOL CSettingClass::InitialFileLoad()
{
	TCHAR chFilePath[256] = { 0, };
	GetModuleFileName(NULL, chFilePath, 256);
	CString strFolderPath(chFilePath);
	strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\'));
	strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\')) + _T("\\Config\\Test.Mot");
	TCHAR strFilename[100];
	_tcscpy_s(strFilename, strFolderPath);
	USES_CONVERSION;
	char* Filename = T2A(strFilename);

	//++ 지정한 Mot파일의 설정값들로 모션보드의 설정값들을 일괄변경 적용합니다.
	if (AxmMotLoadParaAll(Filename) != AXT_RT_SUCCESS)
	{
		AfxMessageBox(_T("Mot File Not Found."));
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CSettingClass::FileSave()
{
	CString temp;
	CString path;
	CFileDialog dlg(FALSE, _T("Mot"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("AXM File (*.mot)|*.mot|All Files (*.*)|*.*||"), this);

	TCHAR chFilePath[256] = { 0, };
	GetModuleFileName(NULL, chFilePath, 256);
	CString strFolderPath(chFilePath);
	strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\'));
	strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\')) + _T("\\Config\\");

	dlg.m_ofn.lpstrInitialDir = strFolderPath;
	if (dlg.DoModal() == IDOK)
	{
		TCHAR strFilename[100];
		temp = dlg.GetPathName();
		_tcscpy_s(strFilename, temp);
		USES_CONVERSION;
		char* Filename = T2A(strFilename);
		//++ 함수 실행 성공시 각각의 모션축 설정값으로 지정한 Mot파일을 생성 및 저장 됩니다.
		if (AXT_RT_SUCCESS != AxmMotSaveParaAll(Filename))
		{
			AfxMessageBox(_T("File save fail!"));
			return FALSE;
		}
		else
		{
			return TRUE;
		}
		temp.ReleaseBuffer();
	}
	else
	{
		return FALSE;
	}
}

BOOL CSettingClass::FileLoad()
{
	CString	temp;
	CFileDialog dlg(TRUE, _T("Mot"), NULL, OFN_HIDEREADONLY, _T("AXM File (*.mot)|*.mot|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		TCHAR strFilename[100];
		temp = dlg.GetPathName();
		_tcscpy_s(strFilename, temp);
		USES_CONVERSION;
		char* Filename = T2A(strFilename);
		//++ 함수 실행 성공시 지정한 Mot파일의 설정값으로 모션축 설정이 일괄 변경됩니다.
		if (AxmMotLoadParaAll(Filename) != AXT_RT_SUCCESS)
		{
			AfxMessageBox(_T("File load fail!"));
			return FALSE;
		}
		else
		{
			return TRUE;
		}			
	}
	else
	{
		return FALSE;
	}
}

DWORD CSettingClass::GetPulseOutMethod(long m_lAxisNo)
{
	DWORD dwMethod;
	DWORD dwRetCode = AxmMotGetPulseOutMethod(m_lAxisNo, &dwMethod);
	if (dwRetCode != AXT_RT_SUCCESS)	dwMethod = OneHighLowHigh;
	return dwMethod;
}

DWORD CSettingClass::GetEncInputMethod(long m_lAxisNo)
{
	DWORD dwMethod;
	DWORD dwRetCode = AxmMotGetEncInputMethod(m_lAxisNo, &dwMethod);
	if (dwRetCode != AXT_RT_SUCCESS)	dwMethod = ObverseUpDownMode;
	return dwMethod;
}	

void CSettingClass::GetVel(long m_lAxisNo, double *dMaxVel, double *dMinVel)
{
	DWORD dwRetMAxCode = AxmMotGetMaxVel(m_lAxisNo, dMaxVel);	
	DWORD dwRetMinCode = AxmMotGetMinVel(m_lAxisNo, dMinVel);
	if (dwRetMAxCode != AXT_RT_SUCCESS)
		if (!dMaxVel == NULL)
		{
			*dMaxVel = 1.0;
		}
	if (dwRetMinCode != AXT_RT_SUCCESS)
		if (!dMinVel == NULL)
		{
			*dMinVel = 1.0;
		}
}

//double CSettingClass::GetMaxVel(long m_lAxisNo)
//{
//	double dMaxVel;
//	DWORD dwRetCode = AxmMotGetMaxVel(m_lAxisNo, &dMaxVel);
//	if (dwRetCode != AXT_RT_SUCCESS)	dMaxVel = 1.0;
//	return dMaxVel;
//}
//double CSettingClass::GetMinVel(long m_lAxisNo)
//{
//	double dMinVel;
//	DWORD dwRetCode = AxmMotGetMinVel(m_lAxisNo, &dMinVel);
//	if (dwRetCode != AXT_RT_SUCCESS)	dMinVel = 1.0;
//	return dMinVel;
//}

void CSettingClass::GetMoveUnitPerPulse(long m_lAxisNo, double *dUnit, long *lPulse)
{
	double Unit;
	long Pulse;
	DWORD dwRetCode = AxmMotGetMoveUnitPerPulse(m_lAxisNo, &Unit, &Pulse);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		if (!dUnit == NULL)
		{			
			*dUnit = 1.0;
		}
		if (!lPulse == NULL)
		{
			*lPulse = 1;
		}
	}
	else
	{
		if (!dUnit == NULL)
		{
			*dUnit = Unit;
		}
		if (!lPulse == NULL)
		{
			*lPulse = Pulse;
		}
	}
}

DWORD CSettingClass::GetServoAlarm(long m_lAxisNo)
{
	DWORD dwStatus;
	DWORD dwRetCode = AxmSignalGetServoAlarm(m_lAxisNo, &dwStatus);
	if (dwRetCode != AXT_RT_SUCCESS)	dwStatus = UNUSED;
	//if (dwStatus != UNUSED)
	//{
	//	AxmSignalReadServoAlarm(m_lAxisNo, &dwStatus);
	//}
	return dwStatus;
}

DWORD CSettingClass::GetInpos(long m_lAxisNo)
{
	DWORD dwStatus;
	DWORD dwRetCode = AxmSignalGetInpos(m_lAxisNo, &dwStatus);
	if (dwRetCode != AXT_RT_SUCCESS)	dwStatus = UNUSED;
	if (dwStatus != UNUSED)
	{
		AxmSignalReadInpos(m_lAxisNo, &dwStatus);
	}
	return dwStatus;
}

DWORD CSettingClass::GetStop(long m_lAxisNo)
{
	DWORD dwStopMode = 0;
	DWORD dwStatus;
	DWORD dwRetCode = AxmSignalGetStop(m_lAxisNo, &dwStopMode, &dwStatus);
	if (dwRetCode != AXT_RT_SUCCESS)	dwStatus = UNUSED;
	if (dwStatus != UNUSED)
	{
		AxmSignalGetStop(m_lAxisNo, 0, &dwStatus);
	}
	return dwStatus;
}

DWORD CSettingClass::GetPEndLimit(long m_lAxisNo)
{
	DWORD dwPositiveLevel;
	DWORD dwRetCode = AxmSignalGetLimit(m_lAxisNo, NULL, &dwPositiveLevel, NULL);
	if (dwRetCode != AXT_RT_SUCCESS)	dwPositiveLevel = UNUSED;
	if (dwPositiveLevel != UNUSED)
	{
		AxmSignalReadLimit(m_lAxisNo, &dwPositiveLevel, NULL);
	}
	return dwPositiveLevel;
}
DWORD CSettingClass::GetMEndLimit(long m_lAxisNo)
{
	DWORD dwNegativeLevel;
	DWORD dwRetCode = AxmSignalGetLimit(m_lAxisNo, NULL, NULL, &dwNegativeLevel);
	if (dwRetCode != AXT_RT_SUCCESS)	dwNegativeLevel = UNUSED;
	if (dwNegativeLevel != UNUSED)
	{
		AxmSignalReadLimit(m_lAxisNo, NULL, &dwNegativeLevel);
	}
	return dwNegativeLevel;
}


DWORD CSettingClass::GetServoOnLevel(long m_lAxisNo)
{
	DWORD dwLevel;
	DWORD dwRetCode = AxmSignalGetServoOnLevel(m_lAxisNo, &dwLevel);
	return dwLevel;
}

DWORD CSettingClass::GetAbsRelMode(long m_lAxisNo)
{
	DWORD dwAbsRelMode;
	DWORD dwRetCode = AxmMotGetAbsRelMode(m_lAxisNo, &dwAbsRelMode);
	if (dwRetCode != AXT_RT_SUCCESS)	dwAbsRelMode = POS_ABS_MODE;
	return dwAbsRelMode;
}

DWORD CSettingClass::GetProfileMode(long m_lAxisNo)
{
	DWORD dwProfileMode;
	DWORD dwRetCode = AxmMotGetProfileMode(m_lAxisNo, &dwProfileMode);
	if (dwRetCode != AXT_RT_SUCCESS)	dwProfileMode = SYM_TRAPEZOIDE_MODE;
	return dwProfileMode;
}
DWORD CSettingClass::GetHomeSignalLevel(long m_lAxisNo)
{
	DWORD dwLevel;
	AxmHomeGetSignalLevel(m_lAxisNo, &dwLevel);
	return dwLevel;
}

void CSettingClass::GetHomeMethod(long m_lAxisNo, long *lHmDir, DWORD *dwHomeSignal, DWORD *dwZphas, double *dHomeClrTime, double *dHomeOffset)
{
	long HmDir;
	DWORD HomeSignal, Zphas;
	double HomeClrTime, HomeOffset;
	AxmHomeGetMethod(m_lAxisNo, &HmDir, &HomeSignal, &Zphas, &HomeClrTime, &HomeOffset);
	if (HmDir < 0)
	{
		HmDir = 0;
	}
	if (HomeSignal < 0)
	{
		HomeSignal = 0;
	}
	if (Zphas < 0)
	{
		Zphas = 0;
	}
	if (HomeClrTime < 0)
	{
		HomeClrTime = 1000.000;
	}
	if (HomeOffset < 0)
	{
		HomeOffset = 0.000;
	}
	if (!lHmDir == NULL)
	{
		*lHmDir = HmDir;
	}
	if (!dwHomeSignal == NULL)
	{
		*dwHomeSignal = HomeSignal;
	}

	if (!dwZphas == NULL)
	{
		*dwZphas = Zphas;
	}

	if (!dHomeClrTime == NULL)
	{
		*dHomeClrTime = HomeClrTime;
	}

	if (!dHomeOffset == NULL)
	{
		*dHomeOffset = HomeOffset;
	}
}

void  CSettingClass::GetHomeVel(long m_lAxisNo, double *dVelFirst, double *dVelSecond, double *dVelThird, double *dVelLast, double *dAccFirst, double *dAccSecond)
{
	double	dHomeClrTime = 1000.0, dHomeOffset = 0.0;

	double VelFirst, VelSecond, VelThird, VelLast, AccFirst, AccSecond;
	AxmHomeGetVel(m_lAxisNo, &VelFirst, &VelSecond, &VelThird, &VelLast, &AccFirst, &AccSecond);
	if (VelFirst < 0)
	{
		VelFirst = 10000.000;
	}
	if (VelSecond < 0)
	{
		VelSecond = 5000.000;
	}
	if (VelThird < 0)
	{
		VelThird = 2500.000;
	}
	if (VelLast < 0)
	{
		VelLast = 1000.000;
	}
	if (AccFirst < 0)
	{
		AccFirst = 100000.000;
	}
	if (AccSecond < 0)
	{
		AccSecond = 100000.000;
	}
	if (!dVelFirst == NULL)
	{
		*dVelFirst = VelFirst;
	}

	if (!dVelSecond == NULL)
	{
		*dVelSecond = VelSecond;
	}

	if (!dVelThird == NULL)
	{
		*dVelThird = VelThird;
	}

	if (!dVelLast == NULL)
	{
		*dVelLast = VelLast;
	}

	if (!dAccFirst == NULL)
	{
		*dAccFirst = AccFirst;
	}
	if (!dAccSecond == NULL)
	{
		*dAccSecond = AccSecond;
	}
}


void CSettingClass::SetMotPulseOutMethod(long lAxisNo, DWORD uMethod)
{
	AxmMotSetPulseOutMethod(lAxisNo, uMethod);
}

void CSettingClass::SetMotEncInputMethod(long lAxisNo, DWORD uMethod)
{
	AxmMotSetEncInputMethod(lAxisNo, uMethod);
}

void CSettingClass::SetServoAlarm(long lAxisNo, DWORD uUse)
{
	AxmSignalSetServoAlarmResetLevel(lAxisNo, uUse);
	//AxmSignalSetServoAlarm(lAxisNo, uUse);
}

void CSettingClass::SetMotAbsRelMode(long lAxisNo, DWORD uAbsRelMode)
{
	AxmMotSetAbsRelMode(lAxisNo, uAbsRelMode);
}

void CSettingClass::SetMotProfileMode(long lAxisNo, DWORD uProfileMode)
{
	AxmMotSetProfileMode(lAxisNo, uProfileMode);
}

void CSettingClass::SetMotVel(long lAxisNo, double MaxVel, double MinVel)
{
	if (MaxVel != NULL)
	{
		AxmMotSetMaxVel(lAxisNo, MaxVel);
	}
	if (MinVel != NULL)
	{
		AxmMotSetMinVel(lAxisNo, MinVel);
	}
}

void CSettingClass::SetMotMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse)
{
	AxmMotSetMoveUnitPerPulse(lAxisNo, dUnit, lPulse);
}

void CSettingClass::SetInpos(long lAxisNo, DWORD uUse)
{
	AxmSignalSetInpos(lAxisNo, uUse);
}

//void CSettingClass::SetServoAlarmResetLevel(long lAxisNo, DWORD uLevel)
//{
//	AxmSignalSetServoAlarmResetLevel(lAxisNo, uLevel);
//
//}
void CSettingClass::SetLimit(long lAxisNo, DWORD uStopMode, DWORD dwLevelLimitP, DWORD dwLevelLimitN)
{
	DWORD dwRetCode = AxmSignalGetLimit(lAxisNo, &uStopMode, NULL, NULL);
	if (dwRetCode == AXT_RT_SUCCESS)
	{
		AxmSignalSetLimit(lAxisNo, uStopMode, dwLevelLimitP, dwLevelLimitN);
	}
}

void CSettingClass::SetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel)
{
	AxmSignalSetStop(lAxisNo, uStopMode, uLevel);
}

void CSettingClass::SetServoOnLevel(long lAxisNo, DWORD uLevel)
{
	AxmSignalSetServoOnLevel(lAxisNo, uLevel);
}

void CSettingClass::SetHomeLevel(long lAxisNo, DWORD uLevel)
{
	AxmHomeSetSignalLevel(lAxisNo, uLevel);
}

void CSettingClass::SetHomeMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset)
{

	AxmHomeSetMethod(lAxisNo, lHmDir, uHomeSignal, uZphas, dHomeClrTime, dHomeOffset);

}

void  CSettingClass::SetHomeVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond)
{
	AxmHomeSetVel(lAxisNo, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
}



// CSettingClass 메시지 처리기입니다.


