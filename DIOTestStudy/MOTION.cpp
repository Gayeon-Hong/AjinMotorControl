// MOTION.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "MOTION.h"
#include "afxdialogex.h"
#include "DIOTestStudyDlg.h"

// MOTION 대화 상자입니다.
#ifdef _M_X64
#pragma comment(lib,  "../AXL(Library)/Library/64Bit/AxL.lib")
#else
#pragma comment(lib,  "../AXL(Library)/Library/32Bit/AxL.lib")
#endif


#define	TM_SENSOR_ID	1000     // ServoOn,Alarm on/off button 현 상태에 따라 색 다르게 보여주기 위함
#define	TM_DISPLAY		100      // Monitoring (cmd pos, act pos, pos err)
#define	TM_HOME_INFO	101      // HomeSearch 결과, 진행율
IMPLEMENT_DYNAMIC(MOTION, CDialogEx)

MOTION::MOTION(CWnd* pParent /*=NULL*/)
	: CDialogEx(MOTION::IDD, pParent)
{
	m_lAxisNo = 0;					// 제어할 축 번호 초기화
	m_lAxisCount = 0;					// 제어 가능한 축갯수 초기화
}

MOTION::~MOTION()
{
}

void MOTION::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_cboSelAxis);
	DDX_Control(pDX, IDC_BTN_JOG_MOVE_N, m_btnJogMoveN);
	DDX_Control(pDX, IDC_BTN_JOG_MOVE_P, m_btnJogMoveP);
	DDX_Control(pDX, IDC_COMBO7, m_cboSelAxis01);
	DDX_Control(pDX, IDC_COMBO6, m_cboSelAxis02);
	DDX_Control(pDX, IDC_COMBO5, m_cboSelAxis03);
	DDX_Control(pDX, IDC_COMBO4, m_cboSelAxis04);
	DDX_Control(pDX, IDC_PRG_HOME_RATE, m_prgHomeRate);
	DDX_Control(pDX, IDC_ServoOn03, m_servoOnbtn);
	DDX_Control(pDX, IDC_Alarm03, m_alarmbtn);
}


BEGIN_MESSAGE_MAP(MOTION, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MOTION::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MOTION::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &MOTION::OnSelchangeCboSelAxis)
	ON_COMMAND_RANGE(IDC_RDO_ABS, IDC_RDO_REL, OnRdoAbsRelMode)
	ON_WM_DESTROY()
	ON_MESSAGE(UM_JOG_BTN_DN, OnJogBtnDn)
	ON_MESSAGE(UM_JOG_BTN_UP, OnJogBtnUp)
	ON_BN_CLICKED(IDC_SSTOP, &MOTION::OnBnClickedSstop)
	ON_BN_CLICKED(IDC_ESTOP, &MOTION::OnBnClickedEstop)
	ON_BN_CLICKED(IDC_STOP, &MOTION::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SINGLEMOVE, &MOTION::OnBnClickedSinglemove)
	ON_BN_CLICKED(IDC_BUTTON8, &MOTION::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &MOTION::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &MOTION::OnBnClickedButton4)
	ON_BN_CLICKED(CntClr, &MOTION::OnBnClickedCntclr)
	ON_BN_CLICKED(IDC_MoveApply, &MOTION::OnBnClickedMoveapply)
	ON_BN_CLICKED(IDC_ServoOn03, &MOTION::OnBnClickedServoon03)
	ON_BN_CLICKED(IDC_Alarm03, &MOTION::OnBnClickedAlarm03)
	ON_BN_CLICKED(IDC_BtnHomeSearch, &MOTION::OnBnClickedBtnhomesearch)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &MOTION::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD2, &MOTION::OnBnClickedButtonReload2)
END_MESSAGE_MAP()


// MOTION 메시지 처리기입니다.
CDIOTestStudyDlg *pDIOTestMotion;
BOOL MOTION::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pMotionClass = new CMotionClass;
	pStatusClass = new CStatusClass;
	pMonitorClass = new CMonitorClass;
	pSettingClass = new CSettingClass;
	pAJinLibrary = new CAJinLibrary;
	pSettingDlg = new SETTING;
	pSettingDlg->Create(IDD_SETTING_DIALOG);
	pStatusDlg = new STATUS;
	pStatusDlg->Create(IDD_STATUS_DIALOG);
	pDIOTestMotion = (CDIOTestStudyDlg *)AfxGetMainWnd();
	AddAxisInfo();

	return TRUE; 
}

#pragma region Setting button, Status button 클릭이벤트 
void MOTION::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!pSettingDlg->bflag)
	{
		pSettingDlg->ShowWindow(SW_SHOW);
		pSettingDlg->bflag = TRUE;
	}
	else
	{
		pSettingDlg->ShowWindow(SW_HIDE);
		pSettingDlg->bflag = FALSE;
	}
}

void MOTION::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!pStatusDlg->bflag)
	{
		pStatusDlg->ShowWindow(SW_SHOW);
		pStatusDlg->bflag = TRUE;
	}
	else
	{
		pStatusDlg->ShowWindow(SW_HIDE);
		pStatusDlg->bflag = FALSE;
	}
}
#pragma endregion

#pragma region 콤보박스에 축 번호 등록 및 초기화, 타이머 설정 

BOOL MOTION::AddAxisInfo()
{
	CString	strData;	
	
	m_lAxisCount = pMotionClass->GetAxisCount();

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
			m_cboSelAxis01.AddString(strData);
			m_cboSelAxis02.AddString(strData);
			m_cboSelAxis03.AddString(strData);
			m_cboSelAxis04.AddString(strData);
		}

		m_cboSelAxis.SetCurSel(0);
		m_cboSelAxis01.SetCurSel(0);
		m_cboSelAxis02.SetCurSel(1);
		m_cboSelAxis03.SetCurSel(2);
		m_cboSelAxis04.SetCurSel(3);
	}
	

	InitControl();							// Control 변수들을 등록하고, 초기 설정값들을 설정합니다.

	m_pThread1 = AfxBeginThread(HomeSearchThread, this);
	if (m_pThread1 == NULL)
		AfxMessageBox(_T("Error!"));
	m_pThread2 = AfxBeginThread(CtrlBtnColorThread, this);
	if (m_pThread2 == NULL)
		AfxMessageBox(_T("Error!"));
	m_pThread3 = AfxBeginThread(MonitoringThread, this);
	if (m_pThread3 == NULL)
		AfxMessageBox(_T("Error!"));
	return TRUE;
}


BOOL MOTION::InitControl()
{
	CString strData;
	m_cboSelAxis.SetCurSel(m_lAxisNo);

	double  dInitPos = 100.0, dInitVel = 10000.0, dInitAccel = 40000.0, dInitDecel = 40000.0;
	// Edit Control에 초기값 설정
	SetDlgItemDouble(IDC_EDT_POS, 1500.000);
	SetDlgItemDouble(IDC_EDT_MOVE_VEL, 100000.000);
	SetDlgItemDouble(IDC_EDT_MOVE_ACC, 200000.000);
	SetDlgItemDouble(IDC_EDT_MOVE_DEC, 200000.000);

	pSettingClass->GetParameter(m_lAxisNo, &dInitPos, &dInitVel, &dInitAccel, &dInitDecel);

	SetDlgItemDouble(IDC_EDT_POS, dInitPos);
	SetDlgItemDouble(IDC_EDT_MOVE_VEL, dInitVel);
	SetDlgItemDouble(IDC_EDT_MOVE_ACC, dInitAccel);
	SetDlgItemDouble(IDC_EDT_MOVE_DEC, dInitDecel);
	SetDlgItemDouble(IDC_STOPVEL, 200.000);
	SetDlgItemText(IDC_PRIORITYMODE, _T("[00] Velocity"));
	m_prgHomeRate.SetRange(0, 100);
	m_prgHomeRate.SetStep(1);
	m_prgHomeRate.SetPos(0);
	return TRUE;
}

#pragma endregion 

#pragma region combobox sel change event - 제어할 축 번호 정보 업데이트
void MOTION::OnSelchangeCboSelAxis()
{
	m_lAxisNo = m_cboSelAxis.GetCurSel();		
}
#pragma endregion

#pragma region 절대 이동을 할 것인지 상대이동을 할 것인지 라디오버튼으로 정함
void MOTION::OnRdoAbsRelMode(UINT nID)
{
	CString strData;
	DWORD	dwAbsRelMode = nID - IDC_RDO_ABS;

	if (dwAbsRelMode == POS_ABS_MODE)
	{
		pMotionClass->SetAbsMode(m_lAxisNo);
	}
	else if (dwAbsRelMode == POS_REL_MODE)
	{
		pMotionClass->SetRelMode(m_lAxisNo);
	}
	pSettingDlg->UpdateState();
}
#pragma endregion 

#pragma region JOG버튼 mouse up/down event
LRESULT MOTION::OnJogBtnDn(WPARAM wParam, LPARAM lParam)
{
	CString strData;

	switch (wParam)
	{
	case IDC_BTN_JOG_MOVE_P:
		//++ 지정한 축을 (+)방향으로 지정한 속도/가속도/감속도로 모션구동합니다.
		pMotionClass->MoveJog(m_lAxisNo, dJogVel, dJogAcc, dJogDec);
		break;

	case IDC_BTN_JOG_MOVE_N:
		//++ 지정한 축을 (-)방향으로 지정한 속도/가속도/감속도로 모션구동합니다.
		pMotionClass->MoveJog(m_lAxisNo, -dJogVel, dJogAcc, dJogDec);
		break;
	}

	return 0;
}
LRESULT MOTION::OnJogBtnUp(WPARAM wParam, LPARAM lParam)
{
	pMotionClass->Estop(m_lAxisNo);
	return 0;
}
#pragma endregion

#pragma region Sstop/Estop/Stop 버튼 클릭 이벤트
//SStop
void MOTION::OnBnClickedSstop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - SStop 버튼 클릭 이벤트 발생"));
	pMotionClass->Sstop(m_lAxisNo);
	
}

//Estop
void MOTION::OnBnClickedEstop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - EStop 버튼 클릭 이벤트 발생"));
	pMotionClass->Estop(m_lAxisNo);
}

//Stop
void MOTION::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Stop 버튼 클릭 이벤트 발생"));
	double dDecel = GetDlgItemDouble(IDC_STOPVEL);
	pMotionClass->Stop(m_lAxisNo, dDecel);
}
#pragma endregion

#pragma region Singlemove 버튼 클릭 이벤트
void MOTION::OnBnClickedSinglemove()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - SingleMove 버튼 클릭 이벤트 발생"));
	pMotionClass->SingleMove(m_lAxisNo, dMovePos, dMoveVel, dMoveAcc, dMoveDec);
}
#pragma endregion 

#pragma region Monitoring(cmd pos, act pos, pos err)값 0.0으로 하기 위한 CntClr 버튼 클릭 이벤트

void MOTION::OnBnClickedButton8()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - 모든 Monitoring(cmd pos, act pos, pos err)값 0.0으로 하기 위한 CntClr 버튼 클릭 이벤트 발생"));
	pMotionClass->ClearAllPosZero(m_cboSelAxis01.GetCurSel());
	pMotionClass->ClearAllPosZero(m_cboSelAxis02.GetCurSel());
	pMotionClass->ClearAllPosZero(m_cboSelAxis03.GetCurSel());
	pMotionClass->ClearAllPosZero(m_cboSelAxis04.GetCurSel());
}

void MOTION::OnBnClickedButton6()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - 0번째 축 Monitoring(cmd pos, act pos, pos err)값 0.0으로 하기 위한 CntClr 버튼 클릭 이벤트 발생"));
	pMotionClass->ClearAllPosZero(m_cboSelAxis01.GetCurSel());
}

void MOTION::OnBnClickedCntclr()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - 1번째 축 Monitoring(cmd pos, act pos, pos err)값 0.0으로 하기 위한 CntClr 버튼 클릭 이벤트 발생"));
	pMotionClass->ClearAllPosZero(m_cboSelAxis02.GetCurSel());
}

void MOTION::OnBnClickedButton4()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - 2번째 축 Monitoring(cmd pos, act pos, pos err)값 0.0으로 하기 위한 CntClr 버튼 클릭 이벤트 발생"));
	pMotionClass->ClearAllPosZero(m_cboSelAxis03.GetCurSel());
}


void MOTION::OnBnClickedButton5()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - 3번째 축 Monitoring(cmd pos, act pos, pos err)값 0.0으로 하기 위한 CntClr 버튼 클릭 이벤트 발생"));
	pMotionClass->ClearAllPosZero(m_cboSelAxis04.GetCurSel());
}


#pragma endregion

#pragma region START Home Search 버튼 클릭 이벤트
void MOTION::OnBnClickedBtnhomesearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Start Home Search 버튼 클릭 이벤트 발생"));

	if (!Searching)
	{
		SearchStart(m_lAxisNo);
		Searching = TRUE;
	}
	else
	{
		AxmMoveEStop(m_lAxisNo);
		Searching = FALSE;
	}
}


void MOTION::SearchStart(int m_lAxisNo)
{
	pMotionClass->StartHomeSearch(m_lAxisNo);
}
#pragma endregion

#pragma region Apply button event (apply를 눌러야 전체 정보에 반영이 됨)
void MOTION::OnBnClickedMoveapply()
{
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - 속도값 적용 버튼 클릭 이벤트 발생"));
	dJogVel = fabs(GetDlgItemDouble(IDC_EDT_MOVE_VEL));
	dJogAcc = fabs(GetDlgItemDouble(IDC_EDT_MOVE_ACC));
	dJogDec = fabs(GetDlgItemDouble(IDC_EDT_MOVE_DEC));

	dMovePos = GetDlgItemDouble(IDC_EDT_POS);
	dMoveVel = GetDlgItemDouble(IDC_EDT_MOVE_VEL);
	dMoveAcc = GetDlgItemDouble(IDC_EDT_MOVE_ACC);
	dMoveDec = GetDlgItemDouble(IDC_EDT_MOVE_DEC);

	pSettingClass->SetParameter(m_lAxisNo, dMovePos, dMoveVel, dMoveAcc, dMoveDec);
}
#pragma endregion 

#pragma region ServoOn, Alarm On/Off를 위한 ServoOn, Alarm 버튼 클릭 이벤트

void MOTION::OnBnClickedServoon03()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pStatusDlg->bSVObtnFlag[m_lAxisNo] == FALSE)
	{
		pStatusDlg->bSVObtnFlag[m_lAxisNo] = TRUE;
		pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Servo On 버튼 클릭 이벤트 발생 -> 켬"));
		if (pStatusDlg->bSVObtnFlag[m_lAxisNo])
		{
			pStatusDlg->m_ServoOn[m_lAxisNo]->SetFaceColor(RGB(0, 255, 0));
			m_servoOnbtn.SetFaceColor(RGB(0, 255, 0));
			pStatusClass->SetServoOn(m_lAxisNo);
		}
	}
	else
	{
		pStatusDlg->bSVObtnFlag[m_lAxisNo] = FALSE;
		pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Servo On 버튼 클릭 이벤트 발생 -> 끔"));
		if (!pStatusDlg->bSVObtnFlag[m_lAxisNo])
		{
			pStatusDlg->m_ServoOn[m_lAxisNo]->SetFaceColor(RGB(255, 0, 0));
			m_servoOnbtn.SetFaceColor(RGB(255, 0, 0));
			pStatusClass->SetServoOFF(m_lAxisNo);
		}
	}

}

void MOTION::OnBnClickedAlarm03()
{
	if (pStatusClass->GetAlarmOnOFF(m_lAxisNo) == LOW)
	{
		pStatusClass->SetAlarmOn(m_lAxisNo);
	}
	else if (pStatusClass->GetAlarmOnOFF(m_lAxisNo) == HIGH)
	{
		pStatusClass->SetAlarmOFF(m_lAxisNo);
		pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Alarm Reset 이벤트 발생"));
	}
	else
	{
	}
}
#pragma endregion

#pragma region Get/Set DlgitemDouble
void MOTION::SetDlgItemDouble(int nID, double value, int nPoint)
{
	CString sTemp, sTemp2;
	sTemp2.Format(_T("%%.%df"), nPoint);
	sTemp.Format(sTemp2, value);
	GetDlgItem(nID)->SetWindowText(sTemp);
}
double MOTION::GetDlgItemDouble(int nID)
{
	double dRet;
	CString sTemp;
	GetDlgItem(nID)->GetWindowText(sTemp);
	dRet = _wtof(sTemp);
	return dRet;
}
#pragma endregion

#pragma region stop thread : ondestroy 이벤트
void MOTION::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pThread1) {
		::SuspendThread(m_pThread1->m_hThread);
	}
	if (NULL != m_pThread2) {
		::SuspendThread(m_pThread2->m_hThread);
	}
	if (NULL != m_pThread3) {
		::SuspendThread(m_pThread3->m_hThread);
	}
}
#pragma endregion

UINT MOTION::MonitoringThread(LPVOID _mothod)
{
	MOTION *Motion = (MOTION*)_mothod;
	CString strData;
	double dCmdPos = 0.0, dActPos = 0.0, dPosErr = 0.0;


	while (true)
	{
	/*00번축*/
		dCmdPos = Motion->pMonitorClass->GetCmdPos(0);
		Motion->SetDlgItemDouble(IDC_STC_CMD_POS01, dCmdPos);

		dActPos = Motion->pMonitorClass->GetActPos(0);
		Motion->SetDlgItemDouble(IDC_STC_ACT_POS01, dActPos);

		dPosErr = Motion->pMonitorClass->GetPosErr(0);
		Motion->SetDlgItemDouble(IDC_STC_POS_ERR01, dPosErr);

	/*01번축*/
		dCmdPos = Motion->pMonitorClass->GetCmdPos(1);
		Motion->SetDlgItemDouble(IDC_STC_CMD_POS02, dCmdPos);

		dActPos = Motion->pMonitorClass->GetActPos(1);
		Motion->SetDlgItemDouble(IDC_STC_ACT_POS02, dActPos);

		dPosErr = Motion->pMonitorClass->GetPosErr(1);
		Motion->SetDlgItemDouble(IDC_STC_POS_ERR02, dPosErr);

	/*02번축*/
		dCmdPos = Motion->pMonitorClass->GetCmdPos(2);
		Motion->SetDlgItemDouble(IDC_STC_CMD_POS03, dCmdPos);

		dActPos = Motion->pMonitorClass->GetActPos(2);
		Motion->SetDlgItemDouble(IDC_STC_ACT_POS03, dActPos);

		dPosErr = Motion->pMonitorClass->GetPosErr(2);
		Motion->SetDlgItemDouble(IDC_STC_POS_ERR03, dPosErr);

	/*03번축*/
		dCmdPos = Motion->pMonitorClass->GetCmdPos(3);
		Motion->SetDlgItemDouble(IDC_STC_CMD_POS04, dCmdPos);

		dActPos = Motion->pMonitorClass->GetActPos(3);
		Motion->SetDlgItemDouble(IDC_STC_ACT_POS04, dActPos);

		dPosErr = Motion->pMonitorClass->GetPosErr(3);
		Motion->SetDlgItemDouble(IDC_STC_POS_ERR04, dPosErr);

	Sleep(TM_DISPLAY);
}
	return 0;
}

UINT MOTION::HomeSearchThread(LPVOID _mothod)
{
	DWORD	dwState, dwRetCode = 0, dwStepMain = 0, dwStepSub = 0; 

	MOTION *Motion = (MOTION*)_mothod;

	while (true)
	{
		//지정한 축의 원점 검색 결과 확인
		dwState = Motion->pMotionClass->GetHomeResult(Motion->m_lAxisNo);

		if (Motion->m_dwOldResult != dwState){
			Motion->SetDlgItemText(IDC_STC_HOME_RESULT, Motion->pMotionClass->HomeSearchResultDw2Csting(dwState));
			Motion->m_dwOldResult = dwState;
		}

		//지정한 축의 원점검색 진행율 확인
		dwStepMain = Motion->pMotionClass->GetHomeMainRate(Motion->m_lAxisNo);
		dwStepSub = Motion->pMotionClass->GetHomeSubRate(Motion->m_lAxisNo);
		if (dwStepMain > -1)
		{
			if (Motion->GetDlgItemInt(IDC_STC_HOME_STEP_MAIN) != (UINT)dwStepMain)
				Motion->SetDlgItemInt(IDC_STC_HOME_STEP_MAIN, dwStepMain);
		}
		if (dwStepSub > -1)
		{
			if (Motion->GetDlgItemInt(IDC_STC_HOME_STEP_SUB) != (UINT)dwStepSub)
				Motion->SetDlgItemInt(IDC_STC_HOME_STEP_SUB, dwStepSub);
		}
		Motion->m_prgHomeRate.SetPos(dwStepSub);

		Sleep(TM_HOME_INFO);
	}
	return 0;
}


UINT MOTION::CtrlBtnColorThread(LPVOID _mothod)
{
	DWORD	dwState, dwRetCode = 0, dwStepMain = 0, dwStepSub = 0;

	MOTION *Motion = (MOTION*)_mothod;

	while (true)
	{
		dwState = Motion->pStatusClass->GetServoOnOFF(Motion->m_lAxisNo);
		if (dwState == TRUE)
		{
			Motion->m_servoOnbtn.SetFaceColor(RGB(0, 255, 0));
		}
		else
		{
			Motion->m_servoOnbtn.SetFaceColor(RGB(255, 0, 0));
		}

		dwState = Motion->pStatusClass->GetAlarmOnOFF(Motion->m_lAxisNo);
		if (dwState == HIGH)
		{
			Motion->m_alarmbtn.SetFaceColor(RGB(255, 0, 0));
		}
		else if (dwState == LOW)
		{
			Motion->m_alarmbtn.SetFaceColor(RGB(150, 150, 150));
		}
		else
		{
			Motion->m_alarmbtn.SetFaceColor(RGB(100, 100, 100));
		}
		Sleep(10);
	}
	return 0;
}

void MOTION::OnBnClickedButtonSave()
{
	double  dInitPos = 10000.0, dInitVel = 100000.0, dInitAccel = 400000.0, dInitDecel = 400000.0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Parameter Setting값 파일 저장 버튼 클릭 이벤트 발생"));
	if (pSettingClass->FileSave())
	{
		pSettingDlg->UpdateState();
		pSettingClass->GetParameter(m_lAxisNo, &dInitPos, &dInitVel, &dInitAccel, &dInitDecel);

		SetDlgItemDouble(IDC_EDT_POS, dInitPos);
		SetDlgItemDouble(IDC_EDT_MOVE_VEL, dInitVel);
		SetDlgItemDouble(IDC_EDT_MOVE_ACC, dInitAccel);
		SetDlgItemDouble(IDC_EDT_MOVE_DEC, dInitDecel);
	}
}


void MOTION::OnBnClickedButtonReload2()
{
	double  dInitPos = 10000.0, dInitVel = 100000.0, dInitAccel = 400000.0, dInitDecel = 400000.0;
	pAJinLibrary->SaveActionLogFile(_T("Motion Dlg - Parameter Setting값 파일 불러오기 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pSettingClass->FileLoad())
	{
		pSettingDlg->UpdateState();
		pSettingClass->GetParameter(m_lAxisNo, &dInitPos, &dInitVel, &dInitAccel, &dInitDecel);

		SetDlgItemDouble(IDC_EDT_POS, dInitPos);
		SetDlgItemDouble(IDC_EDT_MOVE_VEL, dInitVel);
		SetDlgItemDouble(IDC_EDT_MOVE_ACC, dInitAccel);
		SetDlgItemDouble(IDC_EDT_MOVE_DEC, dInitDecel);
	}
}
