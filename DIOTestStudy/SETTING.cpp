// SETTING.cpp : 구현 파일입니다.
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
// SETTING 대화 상자입니다.

IMPLEMENT_DYNAMIC(SETTING, CDialogEx)

CMotionClass *pMotionClass1;
CSettingClass *psettingClass1;
CAJinLibrary *pAJinLibrarySetting;
CDIOTestStudyDlg *pDIOTestSetting;

SETTING::SETTING(CWnd* pParent /*=NULL*/)
	: CDialogEx(SETTING::IDD, pParent)
{
	m_lAxisNo = 0;					// 제어할 축 번호 초기화
	m_lAxisCount = 0;					// 제어 가능한 축갯수 초기화
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


// SETTING 메시지 처리기입니다.

BOOL SETTING::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	pMotionClass1 = new CMotionClass;
	psettingClass1 = new CSettingClass;
	pAJinLibrarySetting = new CAJinLibrary;
	pDIOTestSetting = (CDIOTestStudyDlg *)AfxGetMainWnd();
	AddAxisSettingInfo(); // 콤보박스들 init
	if(	psettingClass1->InitialFileLoad())
	{
		UpdateState();
	}	
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#pragma region 닫기 버튼 클릭이벤트
void SETTING::OnBnClickedButton2()
{
	ShowWindow(SW_HIDE);
	bflag = FALSE;
}
#pragma endregion

#pragma region 콤보박스 초기화 및 타이머 설정
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
			//축 번호 등록
		}
		//m_cboSelAxis.SetCurSel(0);	
		InitCboList(); //아래 콤보박스들 addstring
		m_cboSelAxis.SetCurSel(m_lAxisNo);
	}
	return TRUE;
}

//아래 콤보박스들 addstring
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
	
	//z상 관련은 항목만
	m_cboZPhaseLev.ResetContent();
	m_cboZPhaseLev.EnableWindow(FALSE); 
	//m_cboZPhaseLev.AddString(_T("00:LOW"));
	//m_cboZPhaseLev.AddString(_T("01:HIGH"));
	m_cboZPhaseLev.AddString(_T("UNUSED"));


	//z상 관련은 항목만
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
	// 축 선택 ComboBox에서 선택된 축번호를 제어할 축 번호 변수에 설정합니다.
	m_lAxisNo = m_cboSelAxis.GetCurSel();		

	//++ 지정한 축번호의 모듈ID를 반환합니다.
	AxmInfoGetAxis(m_lAxisNo, NULL, NULL, NULL);

	UpdateState();
}
#pragma endregion

#pragma region 변화가 있을 때마다 축의 상태 확인해서 combobox 및 edit text에 반영
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

	//++ 지정 축의 펄스 출력방식 설정값을 확인합니다.
	dwMethod = psettingClass1->GetPulseOutMethod(m_lAxisNo);
	ConvertAxmToCombo(&m_cboPulse, dwMethod);

	//++ 지정 축의 엔코더 입력방식 설정값을 확인합니다.
	dwMethod = psettingClass1->GetEncInputMethod(m_lAxisNo);
	ConvertAxmToCombo(&m_cboEncoder, dwMethod);

	//++ 지정 축의 알람 신호 입력 시 비상 정지의 사용 여부 및 신호 입력 레벨을 확인합니다.
	dwUse = psettingClass1->GetServoAlarm(m_lAxisNo);
	ConvertAxmToCombo(&m_cboAlarm, dwUse);

	//++ 지정 축의 초기구동 속도를 확인합니다.
	psettingClass1->GetVel(m_lAxisNo, &dMaxVel, &dMinVel);
	SetDlgItemDouble(IDC_EDT_MIN_VEL, dMinVel);
	SetDlgItemDouble(IDC_EDT_MAX_VEL, dMaxVel);

	//++ 지정 축의 거리/속도/가속도의 제어단위/설정값들을 확인합니다.
	psettingClass1->GetMoveUnitPerPulse(m_lAxisNo, &dUnit, &lPulse);
	SetDlgItemDouble(IDC_EDT_MOVE_UNIT, dUnit);
	SetDlgItemInt(IDC_EDT_MOVE_PULSE, lPulse);

	//++ 지정 축의 Z상 신호 Active Level 설정값을 확인합니다.
	//dwRetCode = AxmSignalGetZphaseLevel(m_lAxisNo, &dwLevel);
	//if (dwRetCode != AXT_RT_SUCCESS)	dwLevel = UNUSED;
	//ConvertAxmToCombo(&m_cboZPhaseLev, dwLevel);

	//++ 지정 축의 Servo Alarm 신호 Active Level/사용유무를 확인합니다.
	//dwRetCode = AxmSignalGetServoAlarm(m_lAxisNo, &dwUse);
	//if (dwRetCode != AXT_RT_SUCCESS)	dwUse = UNUSED;
	//dwUse = psettingClass1->GetServoAlarm(m_lAxisNo);
	//ConvertAxmToCombo(&m_cboAlarm, dwUse);

	//++ 지정 축의 Inposition(위치결정완료) 신호 Active Level/사용유무를 확인합니다.
	dwUse = psettingClass1->GetInpos(m_lAxisNo);
	ConvertAxmToCombo(&m_cboInp, dwUse);

	//++ 지정 축의 Emergency 신호 Active Level/사용유무를 확인합니다.
	dwLevel = psettingClass1->GetStop(m_lAxisNo);
	ConvertAxmToCombo(&m_cboStopLevel, dwLevel);

	//++ 지정 축의 (+/-) End Limit 신호 Active Level/사용유무를 확인합니다.
	dwPositiveLevel = psettingClass1->GetPEndLimit(m_lAxisNo);
	ConvertAxmToCombo(&m_cboELimitP, dwPositiveLevel);

	dwNegativeLevel = psettingClass1->GetMEndLimit(m_lAxisNo);
	ConvertAxmToCombo(&m_cboELimitN, dwNegativeLevel);

	//++ 지정 축의 Servo On/Off 신호 Active Level을 확인합니다.
	dwLevel = psettingClass1->GetServoOnLevel(m_lAxisNo);
	ConvertAxmToCombo(&m_cboServoOn, dwLevel);
	
	//++ 지정한 축에 설정된 구동 좌표계를 확인합니다.
	dwAbsRelMode = psettingClass1->GetAbsRelMode(m_lAxisNo);
	ConvertAxmToCombo(&m_cboAbsRel, dwAbsRelMode);

	//++ 지정한 축에 설정된 구동 프로파일 모드를 확인합니다.
	dwProfileMode = psettingClass1->GetProfileMode(m_lAxisNo);
	ConvertAxmToCombo(&m_cboProfile, dwProfileMode);

	//++ 지정한 축에 원점검색 신호 Active Level을 확인합니다.
	dwLevel = psettingClass1->GetHomeSignalLevel(m_lAxisNo);
	ConvertAxmToCombo(&m_cboHome, dwLevel);

	//++ 지정한 축에 원점검색 방법 설정값을 확인합니다.
	psettingClass1->GetHomeMethod(m_lAxisNo, &lHmDir, &dwHomeSignal, &dwZphas, &dHomeClrTime, &dHomeOffset);
	ConvertAxmToCombo(&m_cboHomeDir, lHmDir);
	ConvertAxmToCombo(&m_cboHomeSignal, dwHomeSignal);
	ConvertAxmToCombo(&m_cboZPhaseUse, dwZphas);
	SetDlgItemDouble(IDC_EDT_HOME_CLR_TIME, dHomeClrTime);
	SetDlgItemDouble(IDC_EDT_HOME_OFFSET, dHomeOffset);

	//++ 지정한 축에 원점검색 속도/가속도 설정값을 확인합니다.
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

#pragma region 적용 버튼 이벤트
void SETTING::OnBnClickedButtonApply()
{
	pAJinLibrarySetting->SaveActionLogFile(_T("Setting Dlg - Parameter 변경 적용 버튼 클릭 이벤트 발생됨"));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//적용 버튼을 눌러서 파라미터 설정하고 파일에 저장하기
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
	//++ 지정 축의 초기 구동속도를 설정합니다.
	//AxmMotSetMinVel(lAxisNo, dVelocityMin);
	//++ 지정 축의 최대 구동속도를 설정합니다.
	//AxmMotSetMaxVel(lAxisNo, dVelocityMax);

	//++ 지정 축의 거리/속도/가속도의 제어단위를 설정합니다.
	psettingClass1->SetMotMoveUnitPerPulse(lAxisNo, dMoveUnit, lMovePulse);

	psettingClass1->SetInpos(lAxisNo, dwUseInp);

	//++ 지정 축의 Alarm Reset 신호 Active Level을 설정합니다.
	//psettingClass1->SetServoAlarmResetLevel(lAxisNo, dwLevelAlmRst);
	//AxmSignalSetServoAlarmResetLevel(lAxisNo, dwLevelAlmRst);

	//++ 지정 축의 (+/-) End Limit 신호 Active Level/사용유무를 확인합니다.
	psettingClass1->SetLimit(lAxisNo, dwStopMode, dwLevelLimitP, dwLevelLimitN);

	//++ 지정 축의 Z상 Active Level을 설정합니다.
	//AxmSignalSetZphaseLevel(lAxisNo, dwLevelZPhase);

	//++ 지정 축의 Emergency 신호 Active Level/사용유무를 설정합니다.
	psettingClass1->SetStop(lAxisNo, 0, dwLevelStop);

	//++ 지정 축의 Servo On/Off 신호 Active Level을 설정합니다.
	psettingClass1->SetServoOnLevel(lAxisNo, dwLevelServoOn);

	// Home Search Setting
	//++ 지정 축의 원점신호 Active Level을 설정합니다.
	psettingClass1->SetHomeLevel(lAxisNo, dwLevelHome);

	//++ 지정 축의 원점검색 관련 정보들을 설정합니다.
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
