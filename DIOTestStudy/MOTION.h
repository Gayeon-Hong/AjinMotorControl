#pragma once
#include "SETTING.h"
#include "STATUS.h"
#include "stdafx.h"
#include "afxwin.h"
#include "JogButton.h"
#include "afxcmn.h"
#include "afxbutton.h"
#include "MotionClass.h"
#include "StatusClass.h"
#include "MonitorClass.h"
#include "SettingClass.h"
#include "AJinLibrary.h"
// MOTION 대화 상자입니다.

class MOTION : public CDialogEx
{
	DECLARE_DYNAMIC(MOTION)

public:
	MOTION(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MOTION();
	long		m_lAxisNo;					// 제어할 축 번호
	long		m_lAxisCount;         		// 유효한 전체 모션축수
	CMotionClass  *pMotionClass;
	CStatusClass  *pStatusClass;
	CMonitorClass *pMonitorClass;
	CSettingClass *pSettingClass;
	CAJinLibrary  *pAJinLibrary;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTION_DIALOG };
	SETTING *pSettingDlg;
	STATUS  *pStatusDlg; 
	
	void		SetDlgItemDouble(int nID, double value, int nPoint = 3);	// double값을 지정한 콘트롤에 설정하는 함수
	double		GetDlgItemDouble(int nID);		

	CWinThread* m_pThread1 = NULL;
	CWinThread* m_pThread2 = NULL;
	CWinThread* m_pThread3 = NULL;
	//bool m_bThread1Start;
	//bool m_bThread2Start;
	//bool m_bThread3Start;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnJogBtnDn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnJogBtnUp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:

	double	dJogVel, dJogAcc, dJogDec; 
	double	dMovePos, dMoveVel, dMoveAcc, dMoveDec;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	BOOL		AddAxisInfo();				// 검색된 축 정보를 ComboBox에 등록/Control들을 초기화하는 함수.
	CComboBox   m_cboSelAxis;
	BOOL        InitControl();
	void        OnSelchangeCboSelAxis();
	BOOL        UpdateState();
	afx_msg void OnCbnSelchangeCombo2();
	void         OnRdoAbsRelMode(UINT nID);
	CJogButton m_btnJogMoveN;
	CJogButton m_btnJogMoveP;
	afx_msg void OnBnClickedSstop();
	afx_msg void OnBnClickedEstop();
	afx_msg void OnBnClickedJuststop();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedSinglemove();
	CComboBox m_cboSelAxis01;
	CComboBox m_cboSelAxis02;
	CComboBox m_cboSelAxis03;
	CComboBox m_cboSelAxis04;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedButton8();
	CProgressCtrl m_prgHomeRate;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCntclr();
	DWORD m_dwOldResult;
	void     SearchStart(int m_lAxisNo);
	BOOL Searching = FALSE;
	afx_msg void OnBnClickedMoveapply();
	afx_msg void OnBnClickedServoon03();
	afx_msg void OnBnClickedAlarm03();
	CMFCButton m_servoOnbtn;
	CMFCButton m_alarmbtn;
	afx_msg void OnBnClickedBtnhomesearch();
	afx_msg void OnDestroy();
	static UINT HomeSearchThread(LPVOID _mothod);
	static UINT CtrlBtnColorThread(LPVOID _mothod);
	static UINT MonitoringThread(LPVOID _mothod);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonReload2();
};
