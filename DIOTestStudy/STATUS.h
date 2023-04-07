#pragma once
#include "afxwin.h"


// STATUS ��ȭ �����Դϴ�.

class STATUS : public CDialogEx
{
	DECLARE_DYNAMIC(STATUS)

public:
	STATUS(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~STATUS();
	BOOL  bflag = FALSE;
	BOOL  bSVObtnFlag[4];
	BOOL  bAbtnFlag[4];
	DWORD bInMotionFlag[4];
	DWORD bInPositionFlag[4];
	DWORD bHOMEFlag[4];
	DWORD bPELIMFlag[4];
	DWORD bMELIMFlag[4];
	DWORD bEMGFlag[4];
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STATUS_DIALOG };
	long m_lAxisCount;
	CMFCButton  *m_ServoOn[4];
	CMFCButton  *m_Alarm[4];
	CMFCButton  *m_InMotion[4];
	CMFCButton  *m_InPosition[4];
	CMFCButton  *m_HOME[4];
	CMFCButton  *m_PELIM[4];
	CMFCButton  *m_MELIM[4];
	CMFCButton  *m_EMG[4];
	CMFCButton  *m_HomeSearch[4];
	
	CWinThread* m_pThread = NULL;
	bool m_bThreadStart;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSVOClickColorChange(UINT nID);
	afx_msg void OnAClickColorChange(UINT nID);
	virtual BOOL OnInitDialog();
	void InitStatus();
	CListBox m_homestat;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	static UINT StatusThread(LPVOID _mothod);
};
