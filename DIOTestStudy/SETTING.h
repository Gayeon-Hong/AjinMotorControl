#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;

// SETTING 대화 상자입니다.

class SETTING : public CDialogEx
{
	DECLARE_DYNAMIC(SETTING)

public:
	SETTING(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SETTING();
	BOOL bflag = FALSE;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETTING_DIALOG };
	long		m_lAxisNo;					// 제어할 축 번호
	long		m_lAxisCount;         		// 유효한 전체 모션축수
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	DWORD		ConvertComboToAxm(CComboBox *pCboItem);						// ComboBox콘트롤에 현재 선택된 Item Index를 모션보드에 설정할 값으로 변경하는 함수.	
	long		ConvertAxmToCombo(CComboBox *pCboItem, DWORD dwCurData);	// 설정값과 ComboBox콘트롤의 Item과 일치하는 Item을 찾아 선택하는 함수.	

	void		SetDlgItemDouble(int nID, double value, int nPoint = 3);	// double값을 지정한 콘트롤에 설정하는 함수
	double		GetDlgItemDouble(int nID);									// 지정한 콘트롤에 설정되어 있는 텍스트를 double값으로 변환하여 반환하는 함수.
	
	afx_msg void OnBnClickedButtonSave();
};
