#pragma once


// CMonitorClass

class CMonitorClass : public CWnd
{
	DECLARE_DYNAMIC(CMonitorClass)

public:
	CMonitorClass();
	virtual ~CMonitorClass();
	double GetCmdPos(long m_lAxisNo);
	double GetActPos(long m_lAxisNo);
	double GetPosErr(long m_lAxisNo);
	
protected:
	DECLARE_MESSAGE_MAP()
};


