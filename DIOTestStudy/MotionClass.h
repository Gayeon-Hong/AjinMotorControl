#pragma once


// CMotionClass

class CMotionClass : public CWnd
{
	DECLARE_DYNAMIC(CMotionClass)

public:
	CMotionClass();
	virtual ~CMotionClass();
	void SetAbsMode(long m_lAxisNo);
	void SetRelMode(long m_lAxisNo);
	long GetAxisCount();
	void ClearAllPosZero(long m_lAxisNo);
	CString  HomeSearchResultDw2Csting(DWORD dwHomeResult);
	void   SetHomeResult(long m_lAxisNo);
	DWORD  GetHomeResult(long m_lAxisNo);
	BOOL   GetHomeBoolResult(long m_lAxisNo);
	DWORD  GetHomeMainRate(long m_lAxisNo);
	DWORD  GetHomeSubRate(long m_lAxisNo);
	void   MoveJog(long m_lAxisNo, double dJogVel, double dJogAcc, double dJogDec); 
	void   Estop(long m_lAxisNo);
	void   Sstop(long m_lAxisNo);
	void   Stop(long m_lAxisNo, double dDecel);
	void   SingleMove(long m_lAxisNo, double dMovePos, double dMoveVel, double dMoveAcc, double dMoveDec);
	void   MovePos(long m_lAxisNo, double dMovePos, double dMoveVel, double dMoveAcc, double dMoveDec);
	BOOL   StartHomeSearch(long m_lAxisNo);
protected:
	DECLARE_MESSAGE_MAP()
};


