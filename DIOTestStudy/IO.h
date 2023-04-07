#pragma once
// IO ��ȭ �����Դϴ�.
#include "IOClass.h"
class IO : public CDialogEx
{
	DECLARE_DYNAMIC(IO)

public:
	IO(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~IO();
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };
protected:
	CFont m_Font;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int ModuleNo = 0;
	long IInputCounts = 16;
	long IOutputCounts = 16;
	CButton	*m_pchkInput[CONTACT_MAX_COUNT];
	CButton	*m_pchkOutput[CONTACT_MAX_COUNT];
	CButton	*m_pchkInputOffsetText[CONTACT_MAX_COUNT];
	CButton	*m_pchkOutputOffsetText[CONTACT_MAX_COUNT];
	CButton	*m_pchkInputNameText[CONTACT_MAX_COUNT];
	CButton	*m_pchkOutputNameText[CONTACT_MAX_COUNT];
	afx_msg void OnBnClickedCheck41();
	afx_msg void OnClickOutput(UINT nID);
	afx_msg void InitDIOText();
	afx_msg void InitOffsetButton();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
};
