
// DIOTestStudyDlg.h : ��� ����
//

#if !defined(AFX_DIO_TESTDLG_H__A4642ABB_4B93_4E0E_8943_F10D3CFFBCF9__INCLUDED_)
#define AFX_DIO_TESTDLG_H__A4642ABB_4B93_4E0E_8943_F10D3CFFBCF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxcmn.h"
#include "afxwin.h"
#include "IO.h"
#include <vector>
#include "MOTION.h"
#include "IOClass.h"
#include "AJinLibrary.h"
#include "IOClass.h"
#include "SequenceDlg.h"
#include "MotionClass.h"
#include "LogDlg.h"
using namespace std;
// CDIOTestStudyDlg ��ȭ ����
class CDIOTestStudyDlg : public CDialogEx
{
// �����Դϴ�.
public:
	BOOL CloseDevice();
	BOOL OnModuleInfo(long	lModuleNo, short *InputCount);
	BOOL ThreadData();
	BOOL AXTInit();

	CDIOTestStudyDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.


	IO *pDlg1;
	MOTION * pMotionDlg;
	CIOClass *pIOClass;
	CAJinLibrary *pAJLClass;
	SequenceDlg *pSequenceDlg;
	CMotionClass *pMotionClass;
	CLogDlg *pLogDlg;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIOTESTSTUDY_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	void TabControlItem();
	CTabCtrl m_Tab;

	CWinThread* m_pThread = NULL;
	bool m_bThreadStart;
// �����Դϴ�.

protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnTcnSelchangeRetry(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
	
public:
	CStatic m_connstring;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1(); 
	afx_msg void OnBnClickedInformation2();
	afx_msg void OnBnClickedConnbtn();

	static UINT TimeThread(LPVOID _mothod);
	afx_msg void OnBnClickedBtnsq();
};

#endif // !defined(AFX_DIO_TESTDLG_H__A4642ABB_4B93_4E0E_8943_F10D3CFFBCF9__INCLUDED_)