#pragma once
#include "afxbutton.h"
#include "afxwin.h"
#include <fstream>
#include <iostream>

// SequenceDlg 대화 상자입니다.

class SequenceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SequenceDlg)

public:
	SequenceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SequenceDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SEQUENCE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DWORD YisHome;
	BOOL Initializing = FALSE;
	BOOL AutoRunning  = FALSE;
	DWORD	dWaitTimeMS;
	double  dWaitTimeSec;
	double	dReadyPosX;
	double	dWorkReadyPosX;
	double	dWorkPosX_1;
	double	dWorkPosX_2;
	double	dWorkPosX_3;
	double	dReadyPosY;
	double	dWorkReadyPosY;
	double	dWorkPosY_1;
	double	dWorkPosY_2;
	double	dWorkPosY_3;
	double	dReadyPosZ;
	double	dWorkReadyPosZ;
	double	dWorkPosZ_1;
	double	dWorkPosZ_2;
	double	dWorkPosZ_3;
	
	BOOL bEMOFlag = FALSE;
	BOOL bStopFlag = FALSE;

	BOOL XisReady;
	BOOL YisReady;
	BOOL ZisReady;
	BOOL Searching = FALSE;
	BOOL IsInit = FALSE;
	BOOL IsAutoRun = FALSE;
	
	CWinThread* m_pTowrRedBlinkThread = NULL;
	static UINT TowerRedBlinkThread(LPVOID _mothod);
	
	CWinThread* m_ptowerGreenBlinkThread = NULL;
	static UINT TowerGreenBlinkThread(LPVOID _mothod);

	CWinThread* m_pStopThread = NULL;
	static UINT StopThread(LPVOID _mothod);

	CWinThread* m_pHomeThread = NULL;
	static UINT HomeSearchThread(LPVOID _mothod);

	CWinThread* m_pXReadyThread = NULL;
	static UINT XReadyThread(LPVOID _mothod);

	CWinThread* m_pYReadyThread = NULL;
	static UINT YReadyThread(LPVOID _mothod);

	CWinThread* m_pZReadyThread = NULL;
	static UINT ZReadyThread(LPVOID _mothod);

	CWinThread* m_pInitializeThread = NULL;
	static UINT InitializeThread(LPVOID _mothod);

	CWinThread* m_pInitializeColorThread = NULL;
	static UINT InitializeColorThread(LPVOID _mothod);

	CWinThread* m_pAutoRunThread = NULL;
	static UINT SequenceDlg::AutoRunThread(LPVOID _mothod);

	CWinThread* m_pAutoRunAlarmThread = NULL;
	static UINT SequenceDlg::AutoRunAlarmThread(LPVOID _mothod);

	CWinThread* m_pAutoRunBtnColor = NULL;
	static UINT SequenceDlg::AutoRunBtnColorThread(LPVOID _mothod);

	void TowerBlinkStart(int TowerColor);

	BOOL m_bSeqThread;	
	afx_msg void OnDestroy();
	BOOL bflag = FALSE;
	afx_msg void OnBnClickedButtonexit();
	virtual BOOL OnInitDialog(); 
	CMFCButton m_EmoStat;
	CMFCButton m_StopIOBtn;
	afx_msg void OnBnClickedXInitialbtn();
	afx_msg void OnBnClickedYInitialbtn();
	afx_msg void OnBnClickedZInitialbtn();
	CEdit m_XReadyPos;
	CEdit m_XWorkingReadyPos;
	CEdit m_YWorkingReadyPos;
	CEdit m_YReadyPos;
	CEdit m_ZReadyPos;
	CEdit m_ZWorkingReadyPos;
	afx_msg void OnBnClickedInitializebtn();
	afx_msg void OnBnClickedAutorunbtn();
	CMFCButton m_AutoRunBtn;
	CMFCButton m_InitialBtn;
	afx_msg void OnBnClickedApplyBtn();
	afx_msg void OnBnClickedLoadBtn();
	void		SetDlgItemDouble(int nID, double value, int nPoint = 3);	// double값을 지정한 콘트롤에 설정하는 함수
	double		GetDlgItemDouble(int nID);
	void		LoadPosInfo();
	afx_msg void OnBnClickedReadyxmoveBtn();
	afx_msg void OnBnClickedWrxmoveBtn();
	afx_msg void OnBnClickedW1xmoveBtn();
	afx_msg void OnBnClickedW2xmoveBtn();
	afx_msg void OnBnClickedW3xmoveBtn();
	afx_msg void OnClickedReadyymoveBtn();
	afx_msg void OnBnClickedWrymoveBtn();
	afx_msg void OnBnClickedW1ymoveBtn();
	afx_msg void OnBnClickedW2ymoveBtn();
	afx_msg void OnBnClickedW3ymoveBtn();
	afx_msg void OnBnClickedReadyzmoveBtn();
	afx_msg void OnBnClickedWrzmoveBtn();
	afx_msg void OnBnClickedW1zmoveBtn();
	afx_msg void OnBnClickedW2zmoveBtn();
	afx_msg void OnBnClickedW3zmoveBtn();

	void UpdatePosInfo();
	void AutoRunMove(int Axis, double Position, CString CStrPos);
	void AutoRunMove(int Axis1, double Position1, int Axis2, double Position2, CString CStrPos1, CString CStrPos2);

	SYSTEMTIME	CurrentTime;
	SYSTEMTIME	TInitStart;
	SYSTEMTIME	TInitEnd;
	int	TInitTime;
	SYSTEMTIME	TAutoStart;
	SYSTEMTIME	TAutoEnd;
	int	TAutoTime;

	CString StrPosLog;
	void	SavePosLogFile(int Axis, double Position, CString CStrPos);
	void	SavePosLogFile(int Axis, CString Time, CString CStrPos);
	void	SavePosLogFile(CString str);
};
