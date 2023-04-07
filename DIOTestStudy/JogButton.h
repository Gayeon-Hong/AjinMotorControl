#if !defined(AFX_JOGBUTTON_H__A839C01D_C03C_45F4_970E_A697FB261EA6__INCLUDED_)
#define AFX_JOGBUTTON_H__A839C01D_C03C_45F4_970E_A697FB261EA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JogButton.h : header file
//
#include "afxwin.h"
#define UM_JOG_BTN_DN		WM_USER + 1000
#define UM_JOG_BTN_UP		WM_USER + 1001

class CJogButton :
	public CButton
{
	// Construction
public:
	CJogButton();

	// Attributes
public:

private:
	BOOL		m_bChecked;

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJogButton)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CJogButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJogButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOGBUTTON_H__A839C01D_C03C_45F4_970E_A697FB261EA6__INCLUDED_)
