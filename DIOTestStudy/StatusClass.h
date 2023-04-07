#pragma once


// CStatusClass

class CStatusClass : public CWnd
{
	DECLARE_DYNAMIC(CStatusClass)

public:
	CStatusClass();
	virtual ~CStatusClass();
	void SetServoOn(long m_lAxisNo);
	void SetServoOFF(long m_lAxisNo);
	void SetAlarmOn(long m_lAxisNo);
	void SetAlarmOFF(long m_lAxisNo);

	DWORD GetServoOnOFF(long m_lAxisNo);
	DWORD GetAlarmOnOFF(long m_lAxisNo);
	DWORD GetInMotionHL(long m_lAxisNo);
	DWORD GetInPositionIsUse(long m_lAxisNo);
	DWORD GetInPositionHL(long m_lAxisNo);
	DWORD GetHomeOnOFF(long m_lAxisNo);
	DWORD GetPElimitHL(long m_lAxisNo);
	DWORD GetMElimitHL(long m_lAxisNo);
	DWORD GetEMGHL(long m_lAxisNo);
	DWORD GetHomeSearchResult(long m_lAxisNo);
protected:
	DECLARE_MESSAGE_MAP()
};


