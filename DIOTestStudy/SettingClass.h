#pragma once
#include <vector>
using namespace std;

// CSettingClass

class CSettingClass : public CWnd
{
	DECLARE_DYNAMIC(CSettingClass)

public:
	CSettingClass();
	virtual ~CSettingClass();
	void			SetParameter(long m_lAxisNo, double dMovePos, double dMoveVel, double dMoveAcc, double dMoveDec);
	void			GetParameter(long m_lAxisNo, double *dMovePos, double *dMoveVel, double *dMoveAcc, double *dMoveDec);
	BOOL			FileLoad();
	BOOL			FileSave();
	BOOL			InitialFileLoad(); 
	DWORD			GetPulseOutMethod(long m_lAxisNo);
	DWORD			GetEncInputMethod(long m_lAxisNo);
	void			GetVel(long m_lAxisNo, double *dMaxVel, double *dMinVel);
	//double			GetMinVel(long m_lAxisNo);
	//double			GetMaxVel(long m_lAxisNo);
	void            GetMoveUnitPerPulse(long m_lAxisNo, double *dUnit, long *lPulse);
	DWORD			GetServoAlarm(long m_lAxisNo);
	DWORD			GetInpos(long m_lAxisNo);
	DWORD			GetStop(long m_lAxisNo);
	DWORD			GetPEndLimit(long m_lAxisNo);
	DWORD			GetMEndLimit(long m_lAxisNo);
	DWORD			GetServoOnLevel(long m_lAxisNo);
	DWORD			GetAbsRelMode(long m_lAxisNo);
	DWORD			GetProfileMode(long m_lAxisNo);
	DWORD			GetHomeSignalLevel(long m_lAxisNo);
	void			GetHomeMethod(long m_lAxisNo, long *lHmDir, DWORD *dwHomeSignal, DWORD *dwZphas, double *dHomeClrTime, double *dHomeOffset);
	void            GetHomeVel(long m_lAxisNo, double *dVelFirst, double *dVelSecond, double *dVelThird, double *dVelLast, double *dAccFirst, double *dAccSecond);

	void            SetMotPulseOutMethod(long lAxisNo, DWORD uMethod); 
	void			SetMotEncInputMethod(long lAxisNo, DWORD uMethod);
	void SetServoAlarm(long lAxisNo, DWORD uUse);
	void SetMotAbsRelMode(long lAxisNo, DWORD uAbsRelMode);
	void SetMotProfileMode(long lAxisNo, DWORD uProfileMode);
	void SetMotVel(long lAxisNo, double MaxVel, double MinVel);
	void SetMotMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse);
	void SetInpos(long lAxisNo, DWORD uUse);
	void SetServoAlarmResetLevel(long lAxisNo, DWORD uLevel);
	void SetLimit(long lAxisNo, DWORD uStopMode, DWORD dwLevelLimitP, DWORD dwLevelLimitN);
	void SetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel);
	void SetServoOnLevel(long lAxisNo, DWORD uLevel);
	void SetHomeLevel(long lAxisNo, DWORD uLevel);
	void SetHomeMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
	void SetHomeVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
	
protected:
	DECLARE_MESSAGE_MAP()
};


