#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
using namespace std;

#define ALL					0
#define X축					1
#define Y축					2
#define Z축					3
#define READY위치			4
#define WORKREADY위치		5
#define WORKING1위치		6
#define WORKING2위치		7
#define WORKING3위치		8
#define INITIALIZESEQUENCE	9
#define AUTORUNSEQUENCE		10

#define 프로그램시작	1
#define 프로그램종료	2
#define OUTPUTON		3
#define OUTPUTOFF		4
#define IODIALOG		5
#define SEQUENCEDIALOG	6
#define SETTINGDIALOG	7
#define MAINDIALOG		8
#define MOTIONDIALOG	9

// CLogDlg 대화 상자입니다.

class CLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogDlg)

public:
	CLogDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_DIALOG };

	void AddPosLog(CString str, CString CStrCurTime);
	void AddPosLog(int Axis, double Position, CString CStrPos, CString CStrCurTime);
	void AddActionLog(CString str, CString CStrCurTime);

	void AddPosLog(int Axis, CString Time, CString CStrPos, CString CStrCurTime);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_PosLogList;
	CListBox m_ActionLogList;
	CListBox m_PosSelLogList;
	CListBox m_ActionSelLogList;
	afx_msg void OnDtnDatetimechangePosDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeActionDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);
	void LoadTodayPosLog();
	void LoadTodayActionLog(); 
	BOOL IsPosToday();
	BOOL IsActionToday();
	CDateTimeCtrl m_dateTimePos;
	CDateTimeCtrl m_dateTimeAction;
	virtual BOOL OnInitDialog(); 
	void AddIOLog(int ModuleNo, int nPort, BOOL b);
	afx_msg void OnCbnSelchangePosCombo();
	afx_msg void OnCbnSelchangeActionCombo();
	CComboBox m_PosCombo;
	CComboBox m_ActionCombo;

	void AddPosItem();
	void AddActionItem();

	CString PosSelPath;
	CString ActionSelPath;

	int PosFilter;
	int ActionFilter;

	void FilterPosLog();
	void FilterActionLog();
};
