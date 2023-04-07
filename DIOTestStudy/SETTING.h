#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;

// SETTING ��ȭ �����Դϴ�.

class SETTING : public CDialogEx
{
	DECLARE_DYNAMIC(SETTING)

public:
	SETTING(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~SETTING();
	BOOL bflag = FALSE;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETTING_DIALOG };
	long		m_lAxisNo;					// ������ �� ��ȣ
	long		m_lAxisCount;         		// ��ȿ�� ��ü ������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	BOOL		AddAxisSettingInfo();
	BOOL        InitCboList();
	afx_msg void OnBnClickedButton2();
	BOOL         FileLoad();
	BOOL         FileSave();
	BOOL        UpdateState();
	virtual BOOL OnInitDialog();
	CComboBox m_cboSelAxis;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReload();
	CComboBox m_cboHomeSignal;
	CComboBox m_cboZPhaseUse;
	CComboBox m_cboHomeDir;
	CComboBox m_cboPulse;
	CComboBox m_cboEncoder;
	CComboBox m_cboAbsRel;
	CComboBox m_cboProfile;
	CComboBox m_cboAccelUnit;
	CComboBox m_cboInp;
	CComboBox m_cboAlarm;
	CComboBox m_cboServoOn;
	CComboBox m_cboHome;
	CComboBox m_cboStopLevel;
	CComboBox m_cboZPhaseLev;
	CComboBox m_cboELimitP;
	CComboBox m_cboELimitN;
	void		InitSettingFile();
	DWORD		ConvertComboToAxm(CComboBox *pCboItem);						// ComboBox��Ʈ�ѿ� ���� ���õ� Item Index�� ��Ǻ��忡 ������ ������ �����ϴ� �Լ�.	
	long		ConvertAxmToCombo(CComboBox *pCboItem, DWORD dwCurData);	// �������� ComboBox��Ʈ���� Item�� ��ġ�ϴ� Item�� ã�� �����ϴ� �Լ�.	

	void		SetDlgItemDouble(int nID, double value, int nPoint = 3);	// double���� ������ ��Ʈ�ѿ� �����ϴ� �Լ�
	double		GetDlgItemDouble(int nID);									// ������ ��Ʈ�ѿ� �����Ǿ� �ִ� �ؽ�Ʈ�� double������ ��ȯ�Ͽ� ��ȯ�ϴ� �Լ�.
	
	afx_msg void OnBnClickedButtonSave();
};
