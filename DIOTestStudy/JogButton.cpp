// JogButton.cpp : implementation file
//

#include "stdafx.h"
#include "JogButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJogButton

CJogButton::CJogButton()
{
	m_bChecked = FALSE;			// Jog버튼을 상태
}

CJogButton::~CJogButton()
{
}


BEGIN_MESSAGE_MAP(CJogButton, CButton)
	//{{AFX_MSG_MAP(CJogButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJogButton message handlers

void CJogButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bChecked == FALSE)
	{
		GetParent()->SendMessage(UM_JOG_BTN_DN, this->GetDlgCtrlID(), 0);
		m_bChecked = TRUE;
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CJogButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bChecked == TRUE)
	{
		GetParent()->SendMessage(UM_JOG_BTN_UP, this->GetDlgCtrlID(), 0);
		m_bChecked = FALSE;
	}
	CButton::OnLButtonUp(nFlags, point);
}

void CJogButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	if (m_bChecked == TRUE)
	{
		GetParent()->SendMessage(UM_JOG_BTN_UP, this->GetDlgCtrlID(), 0);
		m_bChecked = FALSE;
	}
}
