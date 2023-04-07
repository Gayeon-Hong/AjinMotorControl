// SETTING.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "DIOTestStudyDlg.h"
#include "SETTING.h"
#include "afxdialogex.h"
#include "MOTION.h"
#include "MotionClass.h"
#include "SettingClass.h"
#include "AJinLibrary.h"
// SETTING ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(SETTING, CDialogEx)

CMotionClass *pMotionClass1;
CSettingClass *psettingClass1;
CAJinLibrary *pAJinLibrarySetting;
CDIOTestStudyDlg *pDIOTestSetting;

SETTING::SETTING(CWnd* pParent /*=NULL*/)
	: CDialogEx(SETTING::IDD, pParent)
{
	m_lAxisNo = 0;					// ������ �� ��ȣ �ʱ�ȭ
	m_lAxisCount = 0;					// ���� ������ �హ�� �ʱ�ȭ
}

SETTING::~SETTING()
{
}

void SETTING::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_cboSelAxis);
	DDX_Control(pDX, IDC_COMBO23, m_cboHomeSignal);
	DDX_Control(pDX, IDC_COMBO24, m_cboZPhaseUse);
	DDX_Control(pDX, IDC_COMBO22, m_cboHomeDir);
	DDX_Control(pDX, IDC_COMBO1, m_cboPulse);
	DDX_Control(pDX, IDC_COMBO3, m_cboEncoder);
	DDX_Control(pDX, IDC_COMBO9, m_cboAbsRel);
	DDX_Control(pDX, IDC_COMBO8, m_cboProfile);
	DDX_Control(pDX, IDC_COMBO10, m_cboAccelUnit);
	DDX_Control(pDX, IDC_COMBO13, m_cboInp);
	DDX_Control(pDX, IDC_COMBO12, m_cboAlarm);
	DDX_Control(pDX, IDC_COMBO11, m_cboServoOn);
	DDX_Control(pDX, IDC_COMBO16, m_cboHome);
	DDX_Control(pDX, IDC_COMBO17, m_cboStopLevel);
	DDX_Control(pDX, IDC_COMBO19, m_cboZPhaseLev);
	DDX_Control(pDX, IDC_COMBO20, m_cboELimitP);
	DDX_Control(pDX, IDC_COMBO21, m_cboELimitN);
}


BEGIN_MESSAGE_MAP(SETTING, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &SETTING::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &SETTING::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &SETTING::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// SETTING �޽��� ó�����Դϴ�.

BOOL SETTING::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	pMotionClass1 = new CMotionClass;
	psettingClass1 = new CSettingClass;
	pAJinLibrarySetting = new CAJinLibrary;
	pDIOTestSetting = (CDIOTestStudyDlg *)AfxGetMainWnd();
	AddAxisSettingInfo(); // �޺��ڽ��� init
	if(	psettingClass1->InitialFileLoad())
	{
		UpdateState();
	}	
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

#pragma region �ݱ� ��ư Ŭ���̺�Ʈ
void SETTING::OnBnClickedButton2()
{
	ShowWindow(SW_HIDE);
	bflag = FALSE;
}
#pragma endregion

#pragma region �޺��ڽ� �ʱ�ȭ �� Ÿ�̸� ����
BOOL SETTING::AddAxisSettingInfo()
{
	CString	strData;
	m_lAxisCount = pMotionClass1->GetAxisCount();
	if (m_lAxisCount < 1)
	{
		return FALSE;
	}
	else
	{
		for (int i = 0; i < m_lAxisCount; i++)
		{
			strData.Format(_T("Axis No : %d"), i);
			m_cboSelAxis.AddString(strData);
			//�� ��ȣ ���
		}
		//m_cboSelAxis.SetCurSel(0);	
		InitCboList(); //�Ʒ� �޺��ڽ��� addstring
		m_cboSelAxis.SetCurSel(m_lAxisNo);
	}
	return TRUE;
}

//�Ʒ� �޺��ڽ��� addstring
BOOL SETTING::InitCboList()
{
	m_cboPulse.ResetContent();
	m_cboPulse.EnableWindow(TRUE);
	m_cboPulse.AddString(_T("00:OneHighLowHigh"));
	m_cboPulse.AddString(_T("01:OneHighHighLow"));
	m_cboPulse.AddString(_T("02:OneLowLowHigh"));
	m_cboPulse.AddString(_T("03:OneLowHighLow"));
	m_cboPulse.AddString(_T("04:TwoCcwCwHigh"));
	m_cboPulse.AddString(_T("05:TwoCcwCwLow"));
	m_cboPulse.AddString(_T("06:TwoCwCcwHigh"));
	m_cboPulse.AddString(_T("07:TwoCwCcwLow"));
	m_cboPulse.AddString(_T("08:TwoPhase"));
	m_cboPulse.AddString(_T("09:TwoPhaseReverse"));

	m_cboEncoder.ResetContent();
	m_cboEncoder.EnableWindow(TRUE);
	m_cboEncoder.AddString(_T("00:ObverseUpDownMode"));
	m_cboEncoder.AddString(_T("01:ObverseSqr1Mode"));
	m_cboEncoder.AddString(_T("02:ObverseSqr2Mode"));
	m_cboEncoder.AddString(_T("03:ObverseSqr4Mode"));
	m_cboEncoder.AddString(_T("04:ReverseUpDownMode"));
	m_cboEncoder.AddString(_T("05:ReverseSqr1Mode"));
	m_cboEncoder.AddString(_T("06:ReverseSqr2Mode"));
	m_cboEncoder.AddString(_T("07:ReverseSqr4Mode"));

	m_cboAbsRel.ResetContent();
	m_cboAbsRel.EnableWindow(TRUE);
	m_cboAbsRel.AddString(_T("00:POS_ABS_MODE"));
	m_cboAbsRel.AddString(_T("01:POS_REL_MODE"));

	m_cboProfile.ResetContent();
	m_cboProfile.EnableWindow(TRUE);
	m_cboProfile.AddString(_T("00:SYM_TRAPEZOIDE_MODE"));
	m_cboProfile.AddString(_T("01:ASYM_TRAPEZOIDE_MODE"));
	m_cboProfile.AddString(_T("02:QUASI_S_CURVE_MODE"));
	m_cboProfile.AddString(_T("03:SYM_S_CURVE_MODE"));
	m_cboProfile.AddString(_T("04:ASYM_S_CURVE_MODE"));
	
	//z�� ������ �׸�
	m_cboZPhaseLev.ResetContent();
	m_cboZPhaseLev.EnableWindow(FALSE); 
	//m_cboZPhaseLev.AddString(_T("00:LOW"));
	//m_cboZPhaseLev.AddString(_T("01:HIGH"));
	m_cboZPhaseLev.AddString(_T("UNUSED"));


	//z�� ������ �׸�
	m_cboZPhaseUse.ResetContent();
	m_cboZPhaseUse.EnableWindow(FALSE); 
	//m_cboZPhaseUse.AddString(_T("00:LOW"));
	//m_cboZPhaseUse.AddString(_T("01:HIGH"));
	m_cboZPhaseUse.AddString(_T("UNUSED"));

	m_cboAlarm.ResetContent();
	m_cboAlarm.EnableWindow(TRUE);
	m_cboAlarm.AddString(_T("00:LOW"));
	m_cboAlarm.AddString(_T("01:HIGH"));
	m_cboAlarm.AddString(_T("02:UNUSED"));

	m_cboInp.ResetContent();
	m_cboInp.EnableWindow(TRUE);
	m_cboInp.AddString(_T("00:LOW"));
	m_cboInp.AddString(_T("01:HIGH"));
	m_cboInp.AddString(_T("02:UNUSED"));

	m_cboStopLevel.ResetContent();
	m_cboStopLevel.EnableWindow(TRUE);
	m_cboStopLevel.AddString(_T("00:LOW"));
	m_cboStopLevel.AddString(_T("01:HIGH"));

	m_cboELimitN.ResetContent();
	m_cboELimitN.EnableWindow(TRUE);
	m_cboELimitN.AddString(_T("00:LOW"));
	m_cboELimitN.AddString(_T("01:HIGH"));

	m_cboELimitP.ResetContent();
	m_cboELimitP.EnableWindow(TRUE);
	m_cboELimitP.AddString(_T("00:LOW"));
	m_cboELimitP.AddString(_T("01:HIGH"));

	m_cboServoOn.ResetContent();
	m_cboServoOn.EnableWindow(TRUE);
	m_cboServoOn.AddString(_T("00:LOW"));
	m_cboServoOn.AddString(_T("01:HIGH"));

	m_cboHomeSignal.ResetContent();
	m_cboHomeSignal.EnableWindow(TRUE);
	m_cboHomeSignal.AddString(_T("00:PosEndLimit"));
	m_cboHomeSignal.AddString(_T("01:NegEndLimit"));
	m_cboHomeSignal.AddString(_T("04:HomeSensor"));
	m_cboHomeSignal.AddString(_T("05:EncodZPhase"));

	m_cboHome.ResetContent();
	m_cboHome.EnableWindow(TRUE);
	m_cboHome.AddString(_T("00:LOW"));
	m_cboHome.AddString(_T("01:HIGH"));

	m_cboHomeDir.ResetContent();
	m_cboHomeDir.EnableWindow(TRUE);
	m_cboHomeDir.AddString(_T("00:(-)DIR_CCW"));
	m_cboHomeDir.AddString(_T("01:(+)DIR_CW"));

	//m_cboZPhaseUse.ResetContent();
	//m_cboZPhaseUse.EnableWindow(TRUE);
	//m_cboZPhaseUse.AddString(_T("00:UNUSED"));
	//m_cboZPhaseUse.AddString(_T("01:(+)DIR_CW"));
	//m_cboZPhaseUse.AddString(_T("02:(-)DIR_CCW"));

	//m_cboZPhaseUse.ResetContent();
	//m_cboZPhaseUse.EnableWindow(TRUE);
	//m_cboZPhaseUse.AddString(_T("00:UNIT/SEC"));
	//m_cboZPhaseUse.AddString(_T("01:UNIT/SEC2"));
	return TRUE;
}


#pragma endregion

#pragma region combobox selchange event
void SETTING::OnCbnSelchangeCombo2()
{
	// �� ���� ComboBox���� ���õ� ���ȣ�� ������ �� ��ȣ ������ �����մϴ�.
	m_lAxisNo = m_cboSelAxis.GetCurSel();		

	//++ ������ ���ȣ�� ���ID�� ��ȯ�մϴ�.
	AxmInfoGetAxis(m_lAxisNo, NULL, NULL, NULL);

	UpdateState();
}
#pragma endregion

#pragma region ��ȭ�� ���� ������ ���� ���� Ȯ���ؼ� combobox �� edit text�� �ݿ�
BOOL SETTING::UpdateState()
{
	if (m_lAxisCount < 1)	return FALSE;

	m_lAxisNo = m_cboSelAxis.GetCurSel();
	long	lPulse = 1;
	double  dUnit = 1.0, dMinVel = 1.0, dMaxVel = 100.0;
	double  dSWLimitP = 0.0, dSWLimitN = 0.0;
	DWORD	dwMethod = 0, dwStopMode = 0, dwUse = 0;
	DWORD	dwPositiveLevel = 0, dwNegativeLevel = 0;
	DWORD	dwSWEnable = 0, dwSWStopMode = 0, dwSWRef = 0;
	DWORD	dwLevel = 0, dwAbsRelMode = 0, dwProfileMode = 0;

	long	lHmDir = 0;
	DWORD	dwHomeSignal = 0, dwZphas = 0;
	double	dHomeClrTime = 1000.0, dHomeOffset = 0.0;

	double	dVelFirst = 100.0, dVelSecond = 10.0, dVelThird = 5.0, dVelLast = 1.0, dAccFirst = 400.0, dAccSecond = 40.0;

	//++ ���� ���� �޽� ��¹�� �������� Ȯ���մϴ�.
	dwMethod = psettingClass1->GetPulseOutMethod(m_lAxisNo);
	ConvertAxmToCombo(&m_cboPulse, dwMethod);

	//++ ���� ���� ���ڴ� �Է¹�� �������� Ȯ���մϴ�.
	dwMethod = psettingClass1->GetEncInputMethod(m_lAxisNo);
	ConvertAxmToCombo(&m_cboEncoder, dwMethod);

	//++ ���� ���� �˶� ��ȣ �Է� �� ��� ������ ��� ���� �� ��ȣ �Է� ������ Ȯ���մϴ�.
	dwUse = psettingClass1->GetServoAlarm(m_lAxisNo);
	ConvertAxmToCombo(&m_cboAlarm, dwUse);

	//++ ���� ���� �ʱⱸ�� �ӵ��� Ȯ���մϴ�.
	psettingClass1->GetVel(m_lAxisNo, &dMaxVel, &dMinVel);
	SetDlgItemDouble(IDC_EDT_MIN_VEL, dMinVel);
	SetDlgItemDouble(IDC_EDT_MAX_VEL, dMaxVel);

	//++ ���� ���� �Ÿ�/�ӵ�/���ӵ��� �������/���������� Ȯ���մϴ�.
	psettingClass1->GetMoveUnitPerPulse(m_lAxisNo, &dUnit, &lPulse);
	SetDlgItemDouble(IDC_EDT_MOVE_UNIT, dUnit);
	SetDlgItemInt(IDC_EDT_MOVE_PULSE, lPulse);

	//++ ���� ���� Z�� ��ȣ Active Level �������� Ȯ���մϴ�.
	//dwRetCode = AxmSignalGetZphaseLevel(m_lAxisNo, &dwLevel);
	//if (dwRetCode != AXT_RT_SUCCESS)	dwLevel = UNUSED;
	//ConvertAxmToCombo(&m_cboZPhaseLev, dwLevel);

	//++ ���� ���� Servo Alarm ��ȣ Active Level/��������� Ȯ���մϴ�.
	//dwRetCode = AxmSignalGetServoAlarm(m_lAxisNo, &dwUse);
	//if (dwRetCode != AXT_RT_SUCCESS)	dwUse = UNUSED;
	//dwUse = psettingClass1->GetServoAlarm(m_lAxisNo);
	//ConvertAxmToCombo(&m_cboAlarm, dwUse);

	//++ ���� ���� Inposition(��ġ�����Ϸ�) ��ȣ Active Level/��������� Ȯ���մϴ�.
	dwUse = psettingClass1->GetInpos(m_lAxisNo);
	ConvertAxmToCombo(&m_cboInp, dwUse);

	//++ ���� ���� Emergency ��ȣ Active Level/��������� Ȯ���մϴ�.
	dwLevel = psettingClass1->GetStop(m_lAxisNo);
	ConvertAxmToCombo(&m_cboStopLevel, dwLevel);

	//++ ���� ���� (+/-) End Limit ��ȣ Active Level/��������� Ȯ���մϴ�.
	dwPositiveLevel = psettingClass1->GetPEndLimit(m_lAxisNo);
	ConvertAxmToCombo(&m_cboELimitP, dwPositiveLevel);

	dwNegativeLevel = psettingClass1->GetMEndLimit(m_lAxisNo);
	ConvertAxmToCombo(&m_cboELimitN, dwNegativeLevel);

	//++ ���� ���� Servo On/Off ��ȣ Active Level�� Ȯ���մϴ�.
	dwLevel = psettingClass1->GetServoOnLevel(m_lAxisNo);
	ConvertAxmToCombo(&m_cboServoOn, dwLevel);
	
	//++ ������ �࿡ ������ ���� ��ǥ�踦 Ȯ���մϴ�.
	dwAbsRelMode = psettingClass1->GetAbsRelMode(m_lAxisNo);
	ConvertAxmToCombo(&m_cboAbsRel, dwAbsRelMode);

	//++ ������ �࿡ ������ ���� �������� ��带 Ȯ���մϴ�.
	dwProfileMode = psettingClass1->GetProfileMode(m_lAxisNo);
	ConvertAxmToCombo(&m_cboProfile, dwProfileMode);

	//++ ������ �࿡ �����˻� ��ȣ Active Level�� Ȯ���մϴ�.
	dwLevel = psettingClass1->GetHomeSignalLevel(m_lAxisNo);
	ConvertAxmToCombo(&m_cboHome, dwLevel);

	//++ ������ �࿡ �����˻� ��� �������� Ȯ���մϴ�.
	psettingClass1->GetHomeMethod(m_lAxisNo, &lHmDir, &dwHomeSignal, &dwZphas, &dHomeClrTime, &dHomeOffset);
	ConvertAxmToCombo(&m_cboHomeDir, lHmDir);
	ConvertAxmToCombo(&m_cboHomeSignal, dwHomeSignal);
	ConvertAxmToCombo(&m_cboZPhaseUse, dwZphas);
	SetDlgItemDouble(IDC_EDT_HOME_CLR_TIME, dHomeClrTime);
	SetDlgItemDouble(IDC_EDT_HOME_OFFSET, dHomeOffset);

	//++ ������ �࿡ �����˻� �ӵ�/���ӵ� �������� Ȯ���մϴ�.
	psettingClass1->GetHomeVel(m_lAxisNo, &dVelFirst, &dVelSecond, &dVelThird, &dVelLast, &dAccFirst, &dAccSecond);
	SetDlgItemDouble(IDC_EDT_VEL_FIRST, dVelFirst);
	SetDlgItemDouble(IDC_EDT_VEL_SECOND, dVelSecond);
	SetDlgItemDouble(IDC_EDT_VEL_THIRD, dVelThird);
	SetDlgItemDouble(IDC_EDT_VEL_LAST, dVelLast);
	SetDlgItemDouble(IDC_EDT_ACC_FIRST, dAccFirst);
	SetDlgItemDouble(IDC_EDT_ACC_SECOND, dAccSecond);

	return TRUE;
}
#pragma endregion

#pragma region ���� ��ư �̺�Ʈ
void SETTING::OnBnClickedButtonApply()
{
	pAJinLibrarySetting->SaveActionLogFile(_T("Setting Dlg - Parameter ���� ���� ��ư Ŭ�� �̺�Ʈ �߻���"));

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//���� ��ư�� ������ �Ķ���� �����ϰ� ���Ͽ� �����ϱ�
	int		lAxisNo = m_cboSelAxis.GetCurSel();
	long	lMovePulse = 1;
	DWORD	dwModeProfile = 0, dwUseAlarm;
	DWORD	dwMethodPulse = 0, dwMethodEncoder = 0, dwModeAbsRel = 0;
	double	dVelocityMin = 1.0, dVelocityMax = 100.0, dMoveUnit = 1.0;

	DWORD	dwUseInp;
	DWORD	dwLevelLimitN = 0, dwLevelLimitP = 0, dwLevelZPhase = 0;
	DWORD	dwModeStop = 0, dwLevelStop = 0, dwLevelServoOn = 0;

	long	lHomeDir = 0;
	DWORD	dwHomeSignal = 0, dwLevelHome = 0, dwZphas = 0;
	double dVelFirst = 10000.000, dVelSecond = 5000.000, dVelThird = 2500.000, dVelLast = 1000.000, dAccFirst = 100000.000, dAccSecond = 100000.000;
	double	dHomeClrTime = 1000.0, dHomeOffset = 0.0;

	DWORD	dwUse = 0, dwStopMode = 0, dwSelection = 0;
	double	dPositivePos = 0.0, dNegativePos = 0.0;

	// Pulse/Encoder Method && Move Parameter Setting
	dwMethodPulse = ConvertComboToAxm(&m_cboPulse);
	dwMethodEncoder = ConvertComboToAxm(&m_cboEncoder);
	dwUseAlarm = ConvertComboToAxm(&m_cboAlarm);
	dwModeAbsRel = ConvertComboToAxm(&m_cboAbsRel);
	dwModeProfile = ConvertComboToAxm(&m_cboProfile);
	dVelocityMin = GetDlgItemDouble(IDC_EDT_MIN_VEL);
	dVelocityMax = GetDlgItemDouble(IDC_EDT_MAX_VEL);
	lMovePulse = GetDlgItemInt(IDC_EDT_MOVE_PULSE);
	dMoveUnit = GetDlgItemDouble(IDC_EDT_MOVE_UNIT);

	// Input/Output Signal Setting
	dwUseInp = ConvertComboToAxm(&m_cboInp);
	dwLevelLimitN = ConvertComboToAxm(&m_cboELimitN);
	dwLevelLimitP = ConvertComboToAxm(&m_cboELimitP);
	dwLevelZPhase = ConvertComboToAxm(&m_cboZPhaseLev);
	dwLevelStop = ConvertComboToAxm(&m_cboStopLevel);
	dwLevelServoOn = ConvertComboToAxm(&m_cboServoOn);

	// Home Search Setting
	dwLevelHome = ConvertComboToAxm(&m_cboHome);
	dwHomeSignal = ConvertComboToAxm(&m_cboHomeSignal);
	lHomeDir = ConvertComboToAxm(&m_cboHomeDir);
	dwZphas = ConvertComboToAxm(&m_cboZPhaseUse);
	dHomeClrTime = GetDlgItemDouble(IDC_EDT_HOME_CLR_TIME);
	dHomeOffset = GetDlgItemDouble(IDC_EDT_HOME_OFFSET);

	dVelFirst = GetDlgItemDouble(IDC_EDT_VEL_FIRST);
	dVelSecond = GetDlgItemDouble(IDC_EDT_VEL_SECOND);
	dVelThird = GetDlgItemDouble(IDC_EDT_VEL_THIRD);
	dVelLast = GetDlgItemDouble(IDC_EDT_VEL_LAST);
	dAccFirst = GetDlgItemDouble(IDC_EDT_ACC_FIRST);
	dAccSecond = GetDlgItemDouble(IDC_EDT_ACC_SECOND);

	if (m_cboPulse.IsWindowEnabled() == TRUE)
	{
		psettingClass1->SetMotPulseOutMethod(lAxisNo, dwMethodPulse);
	}

	if (m_cboEncoder.IsWindowEnabled() == TRUE)
	{
		psettingClass1->SetMotEncInputMethod(lAxisNo, dwMethodEncoder);
	}

	if (m_cboAlarm.IsWindowEnabled() == TRUE)
	{
		psettingClass1->SetServoAlarm(lAxisNo, dwUseAlarm);
	}
	if (m_cboAbsRel.IsWindowEnabled() == TRUE)
	{
		psettingClass1->SetMotAbsRelMode(lAxisNo, dwModeAbsRel);
	}
	if (m_cboProfile.IsWindowEnabled() == TRUE)
	{
		psettingClass1->SetMotProfileMode(lAxisNo, dwModeProfile);
	}

	psettingClass1->SetMotVel(lAxisNo, dVelocityMax, dVelocityMin);
	//++ ���� ���� �ʱ� �����ӵ��� �����մϴ�.
	//AxmMotSetMinVel(lAxisNo, dVelocityMin);
	//++ ���� ���� �ִ� �����ӵ��� �����մϴ�.
	//AxmMotSetMaxVel(lAxisNo, dVelocityMax);

	//++ ���� ���� �Ÿ�/�ӵ�/���ӵ��� ��������� �����մϴ�.
	psettingClass1->SetMotMoveUnitPerPulse(lAxisNo, dMoveUnit, lMovePulse);

	psettingClass1->SetInpos(lAxisNo, dwUseInp);

	//++ ���� ���� Alarm Reset ��ȣ Active Level�� �����մϴ�.
	//psettingClass1->SetServoAlarmResetLevel(lAxisNo, dwLevelAlmRst);
	//AxmSignalSetServoAlarmResetLevel(lAxisNo, dwLevelAlmRst);

	//++ ���� ���� (+/-) End Limit ��ȣ Active Level/��������� Ȯ���մϴ�.
	psettingClass1->SetLimit(lAxisNo, dwStopMode, dwLevelLimitP, dwLevelLimitN);

	//++ ���� ���� Z�� Active Level�� �����մϴ�.
	//AxmSignalSetZphaseLevel(lAxisNo, dwLevelZPhase);

	//++ ���� ���� Emergency ��ȣ Active Level/��������� �����մϴ�.
	psettingClass1->SetStop(lAxisNo, 0, dwLevelStop);

	//++ ���� ���� Servo On/Off ��ȣ Active Level�� �����մϴ�.
	psettingClass1->SetServoOnLevel(lAxisNo, dwLevelServoOn);

	// Home Search Setting
	//++ ���� ���� ������ȣ Active Level�� �����մϴ�.
	psettingClass1->SetHomeLevel(lAxisNo, dwLevelHome);

	//++ ���� ���� �����˻� ���� �������� �����մϴ�.
	psettingClass1->SetHomeMethod(lAxisNo, lHomeDir, dwHomeSignal, dwZphas, dHomeClrTime, dHomeOffset);
	psettingClass1->SetHomeVel(lAxisNo, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
}
#pragma endregion

#pragma endregion

#pragma region Combobox <-> AxmText
long SETTING::ConvertAxmToCombo(CComboBox *pCboItem, DWORD dwCurData)
{
	if (pCboItem == NULL)	return 0;

	long	lCount, lSelData;
	CString strText;

	lCount = pCboItem->GetCount();
	if (lCount == 0)	return 0;

	for (int i = 0; i < lCount; i++){
		pCboItem->GetLBText(i, strText);
		lSelData = _wtoi(strText);
		if (lSelData == (long)dwCurData){
			pCboItem->SetCurSel(i);
			return i;
		}
	}

	pCboItem->SetCurSel(0);
	return 0;
}

DWORD SETTING::ConvertComboToAxm(CComboBox *pCboItem)
{
	if (pCboItem == NULL)	return 0;

	CString strText;
	long	lCount, lSelData;

	lCount = pCboItem->GetCount();
	if (lCount == 0)		return 0;

	lSelData = pCboItem->GetCurSel();
	if (lSelData < 0)	return 0;

	pCboItem->GetLBText(lSelData, strText);
	return (DWORD)_wtoi(strText);
}
#pragma endregion

#pragma region Get/Set DlgItemDouble
void SETTING::SetDlgItemDouble(int nID, double value, int nPoint)
{
	CString sTemp, sTemp2;
	sTemp2.Format(_T("%%.%df"), nPoint);
	sTemp.Format(sTemp2, value);
	GetDlgItem(nID)->SetWindowText(sTemp);
}

double SETTING::GetDlgItemDouble(int nID)
{
	double dRet;
	CString sTemp;
	GetDlgItem(nID)->GetWindowText(sTemp);
	dRet = _wtof(sTemp);
	return dRet;
}
#pragma endregion
