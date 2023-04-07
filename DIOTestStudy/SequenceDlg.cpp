// SequenceDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "SequenceDlg.h"
#include "afxdialogex.h"
#include "IOClass.h"
#include "MotionClass.h"
#include "MonitorClass.h"
#include "SettingClass.h"
#include "StatusClass.h"
#include "AJinLibrary.h"
#include "DIOTestStudyDlg.h"

#define EMOOFFSET 10
#define STOPBTNOFFSET 1
#define  TOWERLAMPRED 10
#define  TOWERLAMPYELLOW 11
#define  TOWERLAMPGREEN 12
#define X 0
#define Y 1
#define Z 2
// SequenceDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(SequenceDlg, CDialogEx)
CIOClass *pIOClassSeq;
CMotionClass *pMotionClassSeq;
CMonitorClass *pMonitorClassSeq;
CSettingClass *pSettingClassSeq;
CStatusClass *pStatusClassSeq;
CAJinLibrary *pAJinLibrarySeq;
CDIOTestStudyDlg *pMainDlg;

SequenceDlg::SequenceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SequenceDlg::IDD, pParent)
{

}

SequenceDlg::~SequenceDlg()
{
}


BOOL SequenceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pAJinLibrarySeq = new CAJinLibrary;
	pMainDlg = (CDIOTestStudyDlg *)AfxGetMainWnd();
	m_InitialBtn.SetFaceColor(RGB(150, 150, 150));
	m_AutoRunBtn.SetFaceColor(RGB(150, 150, 150));
	pMainDlg = (CDIOTestStudyDlg *)AfxGetMainWnd();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pStopThread = AfxBeginThread(StopThread, this);
	if (m_pStopThread == NULL)
		AfxMessageBox(_T("Error!"));

	LoadPosInfo();

	UpdatePosInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void SequenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EMO_BTN, m_EmoStat);
	DDX_Control(pDX, IDC_STOP_BTN, m_StopIOBtn);
	DDX_Control(pDX, IDC_STOP_BTN, m_StopIOBtn);
	DDX_Control(pDX, IDC_EDT_XREADYPOS, m_XReadyPos);
	DDX_Control(pDX, IDC_EDT_XWRPOS, m_XWorkingReadyPos);
	DDX_Control(pDX, IDC_EDT_YWRPOS, m_YWorkingReadyPos);
	DDX_Control(pDX, IDC_EDT_YREADYPOS, m_YReadyPos);
	DDX_Control(pDX, IDC_EDT_ZREADYPOS, m_ZReadyPos);
	DDX_Control(pDX, IDC_EDT_ZWRPOS, m_ZWorkingReadyPos);
	DDX_Control(pDX, IDC_AutoRunBtn, m_AutoRunBtn);
	DDX_Control(pDX, IDC_InitializeBtn, m_InitialBtn);
}


BEGIN_MESSAGE_MAP(SequenceDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONEXIT, &SequenceDlg::OnBnClickedButtonexit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_InitializeBtn, &SequenceDlg::OnBnClickedInitializebtn)
	ON_BN_CLICKED(IDC_AutoRunBtn, &SequenceDlg::OnBnClickedAutorunbtn)
	ON_BN_CLICKED(IDC_APPLY_BTN, &SequenceDlg::OnBnClickedApplyBtn)
	ON_BN_CLICKED(IDC_LOAD_BTN, &SequenceDlg::OnBnClickedLoadBtn)
	ON_BN_CLICKED(IDC_READYXMOVE_BTN, &SequenceDlg::OnBnClickedReadyxmoveBtn)
	ON_BN_CLICKED(IDC_WRXMOVE_BTN, &SequenceDlg::OnBnClickedWrxmoveBtn)
	ON_BN_CLICKED(IDC_W1XMOVE_BTN, &SequenceDlg::OnBnClickedW1xmoveBtn)
	ON_BN_CLICKED(IDC_W2XMOVE_BTN, &SequenceDlg::OnBnClickedW2xmoveBtn)
	ON_BN_CLICKED(IDC_W3XMOVE_BTN, &SequenceDlg::OnBnClickedW3xmoveBtn)
	ON_BN_CLICKED(IDC_READYYMOVE_BTN, &SequenceDlg::OnClickedReadyymoveBtn)
	ON_BN_CLICKED(IDC_WRYMOVE_BTN, &SequenceDlg::OnBnClickedWrymoveBtn)
	ON_BN_CLICKED(IDC_W1YMOVE_BTN, &SequenceDlg::OnBnClickedW1ymoveBtn)
	ON_BN_CLICKED(IDC_W2YMOVE_BTN, &SequenceDlg::OnBnClickedW2ymoveBtn)
	ON_BN_CLICKED(IDC_W3YMOVE_BTN, &SequenceDlg::OnBnClickedW3ymoveBtn)
	ON_BN_CLICKED(IDC_READYZMOVE_BTN, &SequenceDlg::OnBnClickedReadyzmoveBtn)
	ON_BN_CLICKED(IDC_WRZMOVE_BTN, &SequenceDlg::OnBnClickedWrzmoveBtn)
	ON_BN_CLICKED(IDC_W1ZMOVE_BTN, &SequenceDlg::OnBnClickedW1zmoveBtn)
	ON_BN_CLICKED(IDC_W2ZMOVE_BTN, &SequenceDlg::OnBnClickedW2zmoveBtn)
	ON_BN_CLICKED(IDC_W3ZMOVE_BTN, &SequenceDlg::OnBnClickedW3zmoveBtn)
END_MESSAGE_MAP()


// SequenceDlg 메시지 처리기입니다.


void SequenceDlg::OnBnClickedButtonexit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - 닫기 버튼 클릭 이벤트 발생"));
	ShowWindow(SW_HIDE);
	bflag = FALSE;
}

UINT SequenceDlg::StopThread(LPVOID _mothod)
{
	
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;

	while (true)
	{
		if (pIOClassSeq->GetInput(0, EMOOFFSET))
		{
			pMotionClassSeq->SetHomeResult(X);
			pMotionClassSeq->SetHomeResult(Y);
			pMotionClassSeq->SetHomeResult(Z);
			
			pMotionClassSeq->Estop(X); 
			pMotionClassSeq->Estop(Y); 
			pMotionClassSeq->Estop(Z);
			if (Sequence->bEMOFlag == FALSE)
			{
				pAJinLibrarySeq->SaveActionLogFile(_T("EMO 버튼이 눌렸습니다"));
			}
			Sequence->bEMOFlag = TRUE;
			Sequence->m_EmoStat.SetFaceColor(RGB(255, 0, 0));
			//위에 빨간 램프
			pIOClassSeq->SetOutput(0, TOWERLAMPRED, TRUE);
			Sequence->IsInit = FALSE;
			Sequence->m_AutoRunBtn.SetFaceColor(RGB(150, 150, 150));
			Sequence->m_InitialBtn.SetFaceColor(RGB(150, 150, 150));
			Sequence->Initializing = FALSE;
		}
		else
		{
			if (Sequence->bEMOFlag == TRUE)
			{
				pAJinLibrarySeq->SaveActionLogFile(_T("EMO 버튼이 꺼졌습니다"));
			}
			Sequence->bEMOFlag = FALSE;
			Sequence->m_EmoStat.SetFaceColor(RGB(150, 150, 150));
			pIOClassSeq->SetOutput(0, TOWERLAMPRED, FALSE);
		}

		if (pIOClassSeq->GetInput(0, STOPBTNOFFSET) || pIOClassSeq->GetOutput(0, STOPBTNOFFSET))
		{
			pMotionClassSeq->Estop(X);
			pMotionClassSeq->Estop(Y);
			pMotionClassSeq->Estop(Z);
			if (Sequence->bStopFlag == FALSE)
			{
				pAJinLibrarySeq->SaveActionLogFile(_T("Stop 버튼이 눌렸습니다"));
			}
			Sequence->bStopFlag = TRUE;
			Sequence->m_StopIOBtn.SetFaceColor(RGB(255, 0, 0));
			Sequence->IsInit = FALSE;
			Sequence->m_AutoRunBtn.SetFaceColor(RGB(150, 150, 150));
			Sequence->m_InitialBtn.SetFaceColor(RGB(150, 150, 150));
		}
		else
		{
			if (Sequence->bStopFlag == TRUE)
			{
				pAJinLibrarySeq->SaveActionLogFile(_T("Stop 버튼이 꺼졌습니다"));
			}
			Sequence->bStopFlag = FALSE;
			Sequence->m_StopIOBtn.SetFaceColor(RGB(150, 150, 150));
		}

		Sleep(1);

	}
	return 0;
}

void SequenceDlg::OnBnClickedInitializebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Initialize 버튼 클릭 이벤트 발생"));
	long	lHomeDir;
	DWORD	dwHomeSignal, dwZphas;
	double dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond;
	double	dHomeClrTime, dHomeOffset;

	if (!bEMOFlag)
	{
		if (Initializing == FALSE)
		{
			Initializing = TRUE;

			SavePosLogFile(_T("Initialize 시작"));

			for (int Axis = X; Axis < Z; Axis++)
			{
				pMotionClassSeq->SetHomeResult(Axis);

				pSettingClassSeq->GetHomeMethod(Axis, &lHomeDir, &dwHomeSignal, &dwZphas, &dHomeClrTime, &dHomeOffset);
				pSettingClassSeq->GetHomeVel(Axis, &dVelFirst, &dVelSecond, &dVelThird, &dVelLast, &dAccFirst, &dAccSecond);

				pSettingClassSeq->SetHomeMethod(Axis, lHomeDir, dwHomeSignal, dwZphas, dHomeClrTime, dHomeOffset);
				pSettingClassSeq->SetHomeVel(Axis, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
			}

			m_pInitializeThread = AfxBeginThread(InitializeThread, this);
			if (m_pInitializeThread == NULL)
				AfxMessageBox(_T("Error!"));

			m_pInitializeColorThread = AfxBeginThread(InitializeColorThread, this);
			if (m_pInitializeColorThread == NULL)
				AfxMessageBox(_T("Error!"));

			YisHome = pMotionClassSeq->GetHomeResult(Y);
			m_pHomeThread = AfxBeginThread(HomeSearchThread, this);
			if (m_pHomeThread == NULL)
				AfxMessageBox(_T("Error!"));
		}
		else
		{
			Initializing = FALSE;
		}
	
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}	
}

UINT SequenceDlg::HomeSearchThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (pMotionClassSeq->GetHomeResult(X) != HOME_SUCCESS ||
		   pMotionClassSeq->GetHomeResult(Y) != HOME_SUCCESS ||
		   pMotionClassSeq->GetHomeResult(Z) != HOME_SUCCESS)
	{
		if (pMotionClassSeq->GetHomeResult(X) != HOME_SUCCESS && pMotionClassSeq->GetHomeResult(X) != HOME_SEARCHING)
		{
			if (!pMotionClassSeq->StartHomeSearch(X))
				return 0;
			Sleep(5000);
		}
		else
		{
			if (pMotionClassSeq->GetHomeResult(X) == HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Y) != HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Z) != HOME_SUCCESS)
			{
				Sequence->SavePosLogFile(X, pMonitorClassSeq->GetCmdPos(X), _T("(Home)"));
				//Sequence->SavePosLogFile(X, pMonitorClassSeq->GetCmdPos(X), _T("(Home)"));
			}
			if (pMotionClassSeq->GetHomeResult(Y) != HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Y) != HOME_SEARCHING)
			{
				if (!pMotionClassSeq->StartHomeSearch(Y))
					return 0;
				Sleep(5000);
			}
			else
			{
				if (pMotionClassSeq->GetHomeResult(X) == HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Y) == HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Z) != HOME_SUCCESS)
				{
					if (pMotionClassSeq->GetHomeResult(Y) != HOME_SUCCESS)
					{
						Sequence->SavePosLogFile(Y, pMonitorClassSeq->GetCmdPos(Y), _T("(Home)"));
					}
					Sequence->YisHome = pMotionClassSeq->GetHomeResult(Y);
				}

				if (pMotionClassSeq->GetHomeResult(Z) != HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Z) != HOME_SEARCHING)
				{
						if(!pMotionClassSeq->StartHomeSearch(Z))
							return 0;
						Sleep(5000);
				}
			}

		}
	} 
	
	if (pMotionClassSeq->GetHomeResult(X) == HOME_SUCCESS &&
		pMotionClassSeq->GetHomeResult(Y) == HOME_SUCCESS &&
		pMotionClassSeq->GetHomeResult(Z) == HOME_SUCCESS)
	{
		if (pMotionClassSeq->GetHomeResult(X) == HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Y) == HOME_SUCCESS && pMotionClassSeq->GetHomeResult(Z) == HOME_SUCCESS)
		{
			Sequence->SavePosLogFile(Z, pMonitorClassSeq->GetCmdPos(Z), _T("(Home)"));
		}

		Sequence->m_pXReadyThread = AfxBeginThread(XReadyThread, Sequence);
		if (Sequence->m_pXReadyThread == NULL)
			AfxMessageBox(_T("Error!"));
		Sequence->m_pYReadyThread = AfxBeginThread(YReadyThread, Sequence);
		if (Sequence->m_pYReadyThread == NULL)
			AfxMessageBox(_T("Error!"));
		Sequence->m_pZReadyThread = AfxBeginThread(ZReadyThread, Sequence);
		if (Sequence->m_pZReadyThread == NULL)
			AfxMessageBox(_T("Error!"));
	}
	return 0;
}

UINT SequenceDlg::XReadyThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	double	dMoveVel, dMoveAcc, dMoveDec;
	pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
	while (Sequence->dReadyPosX != pMonitorClassSeq->GetCmdPos(X))
	{
		pMotionClassSeq->MovePos(X, Sequence->dReadyPosX, dMoveVel, dMoveAcc, dMoveDec);
		Sleep(10);
		if (Sequence->dReadyPosX == pMonitorClassSeq->GetCmdPos(X))
		{
			Sequence->XisReady = TRUE;
			Sequence->SavePosLogFile(X, pMonitorClassSeq->GetCmdPos(X), _T("(Ready Position)"));
		}
		else
		{
			Sequence->XisReady = FALSE;
		}
		Sleep(10);
	}
	return 0;
}

UINT SequenceDlg::YReadyThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	double	dMoveVel, dMoveAcc, dMoveDec;
	pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);	
	while (Sequence->dReadyPosY != pMonitorClassSeq->GetCmdPos(Y))
	{
		pMotionClassSeq->MovePos(Y, Sequence->dReadyPosY, dMoveVel, dMoveAcc, dMoveDec);
		Sleep(10);
		if (Sequence->dReadyPosY == pMonitorClassSeq->GetCmdPos(Y))
		{
			Sequence->YisReady = TRUE;
			Sequence->SavePosLogFile(Y, pMonitorClassSeq->GetCmdPos(Y), _T("(Ready Position)"));
		}
		else
		{
			Sequence->YisReady = FALSE;
		}
		Sleep(10);
	}
	
	return 0;
}


UINT SequenceDlg::ZReadyThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	double	dMoveVel, dMoveAcc, dMoveDec;
	pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
	while (Sequence->dReadyPosZ != pMonitorClassSeq->GetCmdPos(Z))
	{
		pMotionClassSeq->MovePos(Z, Sequence->dReadyPosZ, dMoveVel, dMoveAcc, dMoveDec);
		Sleep(10);
		if (Sequence->dReadyPosZ == pMonitorClassSeq->GetCmdPos(Z))
		{
			Sequence->ZisReady = TRUE;
			Sequence->SavePosLogFile(Z, pMonitorClassSeq->GetCmdPos(Z), _T("(Ready Position)"));
		}
		else
		{
			Sequence->ZisReady = FALSE;
		}
		Sequence->ZisReady = TRUE;
		Sleep(10);
	}
	
	return 0;
}

UINT SequenceDlg::InitializeThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (true)
	{
		if (Sequence->XisReady == TRUE && 
			Sequence->YisReady == TRUE && 
			Sequence->ZisReady == TRUE &&
			pMotionClassSeq->GetHomeResult(X) == HOME_SUCCESS &&
			pMotionClassSeq->GetHomeResult(Y) == HOME_SUCCESS &&
			pMotionClassSeq->GetHomeResult(Z) == HOME_SUCCESS )
		{
				Sequence->IsInit = TRUE;
				AfxMessageBox(_T("Initialize 완료!"));
				Sequence->SavePosLogFile(_T("Initialize 완료"));
				if (NULL != Sequence->m_pXReadyThread)
				{
					::SuspendThread(Sequence->m_pXReadyThread->m_hThread);
				}
				if (NULL != Sequence->m_pYReadyThread)
				{
					::SuspendThread(Sequence->m_pYReadyThread->m_hThread);
				}
				if (NULL != Sequence->m_pZReadyThread)
				{
					::SuspendThread(Sequence->m_pZReadyThread->m_hThread);
				}

				pIOClassSeq->SetOutput(0, TOWERLAMPYELLOW, TRUE);
				return 0;		
		}
		else
		{
			Sequence->IsInit = FALSE;
			Sequence->IsAutoRun = FALSE;
		}
		Sleep(10);
	}
	return 0;
}

UINT SequenceDlg::InitializeColorThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (TRUE)
	{
		if (Sequence->IsInit)
			Sequence->m_InitialBtn.SetFaceColor(RGB(0, 255, 0));
		else
		{
			Sequence->m_InitialBtn.SetFaceColor(RGB(150, 150, 150));
		}
		Sleep(10);
	}
	
	return 0;
}


void SequenceDlg::OnBnClickedAutorunbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Auto Run 버튼 클릭 이벤트 발생"));
	if (!bEMOFlag)
	{
		if (AutoRunning == FALSE)
		{
			AutoRunning = TRUE;
			
			m_pAutoRunAlarmThread = AfxBeginThread(AutoRunAlarmThread, this);
			if (m_pAutoRunAlarmThread == NULL)
				AfxMessageBox(_T("Error!"));

			m_pAutoRunBtnColor = AfxBeginThread(AutoRunBtnColorThread, this);
			if (m_pAutoRunBtnColor == NULL)
				AfxMessageBox(_T("Error!"));

			if (IsInit)
			{
				pIOClassSeq->SetOutput(0, TOWERLAMPYELLOW, FALSE);
				TowerBlinkStart(TOWERLAMPGREEN);

				SavePosLogFile(_T("AutoRun 시작"));

				m_pAutoRunThread = AfxBeginThread(AutoRunThread, this);
				if (m_pAutoRunThread == NULL)
					AfxMessageBox(_T("Error!"));

			}
			else
			{
				AfxMessageBox(_T("Intialize가 완료되지 않았습니다! 다시 시도해주세요"));
				TowerBlinkStart(TOWERLAMPRED);
				return;
			}
		}
		else
		{
			AutoRunning = FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}

UINT SequenceDlg::AutoRunThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (true)
	{
		if (pMonitorClassSeq->GetCmdPos(Z) != Sequence->dReadyPosZ || pMonitorClassSeq->GetCmdPos(X) != Sequence->dReadyPosX || pMonitorClassSeq->GetCmdPos(Y) != Sequence->dReadyPosY)
		{
			Sequence->AutoRunMove(Z, Sequence->dReadyPosZ, _T("(Ready Position)"));

			Sequence->AutoRunMove(X, Sequence->dReadyPosX, Y, Sequence->dReadyPosY, _T("(Ready Position)"), _T("(Ready Position)"));
		}
		Sequence->AutoRunMove(Y, Sequence->dWorkReadyPosY, _T("(Work Ready Position)"));

		Sequence->AutoRunMove(X, Sequence->dWorkReadyPosX, _T("(Work Ready Position)"));

		Sequence->AutoRunMove(Z, Sequence->dWorkReadyPosZ, _T("(Work Ready Position)"));

		while (pMonitorClassSeq->GetCmdPos(X) != Sequence->dWorkPosX_2 || pMonitorClassSeq->GetCmdPos(Y) != Sequence->dWorkPosY_2)
		{
			Sequence->AutoRunMove(Y, Sequence->dWorkPosY_2, _T("(Working #2 Position)"));

			Sequence->AutoRunMove(X, Sequence->dWorkPosX_2, _T("(Working #2 Position)"));
			if (pMonitorClassSeq->GetCmdPos(X) == Sequence->dWorkPosX_2 && pMonitorClassSeq->GetCmdPos(Y) == Sequence->dWorkPosY_2)
			{
				Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_2, _T("(Working #2 Position)"));
			}
			else
			{
				continue;
			}
		}

		pIOClassSeq->SetOutput(1, 0, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, TRUE);


		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_3, _T("(Working #3 Position)"));

		pIOClassSeq->SetOutput(1, 0, FALSE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, FALSE);
		pIOClassSeq->SetOutput(1, 1, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, TRUE);

		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_1, _T("(Working #1 Position)"));

		pIOClassSeq->SetOutput(1, 1, FALSE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, FALSE);
		pIOClassSeq->SetOutput(1, 2, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 2, TRUE);

		while (pMonitorClassSeq->GetCmdPos(Z) != Sequence->dWorkReadyPosZ)
		{
			Sequence->AutoRunMove(Z, Sequence->dWorkReadyPosZ, _T("(Work Ready Position)"));

			pIOClassSeq->SetOutput(1, 0, TRUE);
			pMainDlg->pLogDlg->AddIOLog(1, 0, TRUE);
			pIOClassSeq->SetOutput(1, 1, TRUE);
			pMainDlg->pLogDlg->AddIOLog(1, 1, TRUE);
			pIOClassSeq->SetOutput(1, 2, TRUE);
			pMainDlg->pLogDlg->AddIOLog(1, 2, TRUE);

			if (pMonitorClassSeq->GetCmdPos(Z) != Sequence->dWorkReadyPosZ)
			{
				continue;
			}
		}

		while (pMonitorClassSeq->GetCmdPos(X) != Sequence->dWorkPosX_1 || pMonitorClassSeq->GetCmdPos(Y) != Sequence->dWorkPosY_1)
		{
			Sequence->AutoRunMove(Y, Sequence->dWorkPosY_1, _T("(Working #1 Position)"));

			Sequence->AutoRunMove(X, Sequence->dWorkPosX_1, _T("(Working #1 Position)"));

			if (pMonitorClassSeq->GetCmdPos(X) != Sequence->dWorkPosX_1 || pMonitorClassSeq->GetCmdPos(Y) != Sequence->dWorkPosY_1)
			{
				continue;
			}
		}
		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_2, _T("(Working #2 Position)"));

		pIOClassSeq->SetOutput(1, 0, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, TRUE);

		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_3, _T("(Working #3 Position)"));

		pIOClassSeq->SetOutput(1, 0, FALSE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, FALSE);
		pIOClassSeq->SetOutput(1, 1, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, TRUE);

		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_1, _T("(Working #1 Position)"));

		pIOClassSeq->SetOutput(1, 1, FALSE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, FALSE);
		pIOClassSeq->SetOutput(1, 2, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 2, TRUE);

		while (pMonitorClassSeq->GetCmdPos(Z) != Sequence->dWorkReadyPosZ)
		{
			Sequence->AutoRunMove(Z, Sequence->dWorkReadyPosZ, _T("(Work Ready Position)"));

			pIOClassSeq->SetOutput(1, 0, TRUE);
			pMainDlg->pLogDlg->AddIOLog(1, 0, TRUE);
			pIOClassSeq->SetOutput(1, 1, TRUE);
			pMainDlg->pLogDlg->AddIOLog(1, 1, TRUE);
			pIOClassSeq->SetOutput(1, 2, TRUE);
			pMainDlg->pLogDlg->AddIOLog(1, 2, TRUE);

			if (pMonitorClassSeq->GetCmdPos(Z) != Sequence->dWorkReadyPosZ)
			{
				continue;
			}
		}

		while (pMonitorClassSeq->GetCmdPos(X) != Sequence->dWorkPosX_3 || pMonitorClassSeq->GetCmdPos(Y) != Sequence->dWorkPosY_3)
		{
			Sequence->AutoRunMove(Y, Sequence->dWorkPosY_3, _T("(Working #3 Position)"));

			Sequence->AutoRunMove(X, Sequence->dWorkPosX_3, _T("(Working #3 Position)"));

			if (pMonitorClassSeq->GetCmdPos(X) != Sequence->dWorkPosX_3 || pMonitorClassSeq->GetCmdPos(Y) != Sequence->dWorkPosY_3)
			{
				continue;
			}
		}

		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_2, _T("(Working #2 Position)"));

		pIOClassSeq->SetOutput(1, 0, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, TRUE);

		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_3, _T("(Working #3 Position)"));

		pIOClassSeq->SetOutput(1, 0, FALSE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, FALSE);
		pIOClassSeq->SetOutput(1, 1, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, TRUE);

		Sequence->AutoRunMove(Z, Sequence->dWorkPosZ_1, _T("(Working #1 Position)"));

		pIOClassSeq->SetOutput(1, 1, FALSE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, FALSE);
		pIOClassSeq->SetOutput(1, 2, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 2, TRUE);

		Sequence->AutoRunMove(Z, Sequence->dWorkReadyPosZ, _T("(Work Ready Position)"));

		pIOClassSeq->SetOutput(1, 0, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 0, TRUE);
		pIOClassSeq->SetOutput(1, 1, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 1, TRUE);
		pIOClassSeq->SetOutput(1, 2, TRUE);
		pMainDlg->pLogDlg->AddIOLog(1, 2, TRUE);

		while (pMonitorClassSeq->GetCmdPos(X) != Sequence->dReadyPosX ||
			pMonitorClassSeq->GetCmdPos(Y) != Sequence->dReadyPosY ||
			pMonitorClassSeq->GetCmdPos(Z) != Sequence->dReadyPosZ)
		{
			Sequence->AutoRunMove(Z, Sequence->dReadyPosZ, _T("(Ready Position)"));

			Sequence->AutoRunMove(X, Sequence->dReadyPosX, Y, Sequence->dReadyPosY, _T("(Ready Position)") , _T("(Ready Position)"));


			if (pMonitorClassSeq->GetCmdPos(X) == Sequence->dReadyPosX &&
				pMonitorClassSeq->GetCmdPos(Y) == Sequence->dReadyPosY &&
				pMonitorClassSeq->GetCmdPos(Z) == Sequence->dReadyPosZ)
			{
				AfxMessageBox(_T("AutoRun 완료!"));
				Sequence->SavePosLogFile(_T("AutoRun 완료"));
				Sequence->IsAutoRun = TRUE;
				return 0;
			}

			else
			{
				Sequence->IsAutoRun = FALSE;
			}
		}
	}


	return 0;
}

void SequenceDlg::AutoRunMove(int Axis, double Position, CString CStrPos)
{
	double	dMoveVel, dMoveAcc, dMoveDec;

	switch (Axis)
	{
	case X:
		pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
		break;
	case Y:
		pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
		break;
	case Z:
		pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
		break;
	default:
		break;
	}

	while (Position != pMonitorClassSeq->GetCmdPos(Axis))
	{
		pMotionClassSeq->MovePos(Axis, Position, dMoveVel, dMoveAcc, dMoveDec);
		Sleep(dWaitTimeMS);
	}
	SavePosLogFile(Axis, pMonitorClassSeq->GetCmdPos(Axis), CStrPos);
}


void SequenceDlg::AutoRunMove(int Axis1, double Position1, int Axis2, double Position2 , CString CStrPos1, CString CStrPos2)
{
	double	dMoveVel1, dMoveAcc1, dMoveDec1;
	double	dMoveVel2, dMoveAcc2, dMoveDec2;

	switch (Axis1)
	{
	case X:
		pSettingClassSeq->GetParameter(X, NULL, &dMoveVel1, &dMoveAcc1, &dMoveDec1);
		break;
	case Y:
		pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel1, &dMoveAcc1, &dMoveDec1);
		break;
	case Z:
		pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel1, &dMoveAcc1, &dMoveDec1);
		break;
	default:
		break;
	}
	switch (Axis2)
	{
	case X:
		pSettingClassSeq->GetParameter(X, NULL, &dMoveVel2, &dMoveAcc2, &dMoveDec2);
		break;
	case Y:
		pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel2, &dMoveAcc2, &dMoveDec2);
		break;
	case Z:
		pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel2, &dMoveAcc2, &dMoveDec2);
		break;
	default:
		break;
	}

	while (Position1 != pMonitorClassSeq->GetCmdPos(Axis1) || Position2 != pMonitorClassSeq->GetCmdPos(Axis2))
	{
		pMotionClassSeq->MovePos(Axis1, Position1, dMoveVel1, dMoveAcc1, dMoveDec1);
		pMotionClassSeq->MovePos(Axis2, Position2, dMoveVel2, dMoveAcc2, dMoveDec2);
		Sleep(dWaitTimeMS);
	}
	SavePosLogFile(Axis1, pMonitorClassSeq->GetCmdPos(Axis1), CStrPos1);
	SavePosLogFile(Axis2, pMonitorClassSeq->GetCmdPos(Axis2), CStrPos2);
}

void SequenceDlg::SavePosLogFile(int Axis, double Position, CString CStrPos)
{
	CString CstrAxis, CStrCurTime;
	switch (Axis)
	{
	case X:
		CstrAxis = _T("X축");
		break;
	case Y:
		CstrAxis = _T("Y축");
		break;
	case Z:
		CstrAxis = _T("Z축");
		break;
	}

	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	CString filePath, StrPosition, directoryPath;
	CStdioFile File;
	BOOL bOpen;

	setlocale(LC_ALL, "Korean");
	directoryPath.Format(_T("../Log\\%4d년"), CurrentTime.wYear);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월"), CurrentTime.wYear, CurrentTime.wMonth);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	CreateDirectory(directoryPath, NULL);

	filePath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\Position.txt"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	bOpen = File.Open(filePath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite);
	if (!bOpen)
	{
		SavePosLogFile(Axis, Position, CStrPos);
		//AfxMessageBox(_T("LOG File Create/Open Error"));
	}
	else
	{
		CStrCurTime = pAJinLibrarySeq->StrCurrentTime();
		File.SeekToEnd();
		StrPosition.Format(_T("%s| %s 위치 | %f %s \n"), CStrCurTime, CstrAxis, Position, CStrPos);
		File.WriteString(StrPosition);
		File.Close();
	}
	pMainDlg->pLogDlg->AddPosLog(Axis, Position, CStrPos, CStrCurTime);
}

void SequenceDlg::SavePosLogFile(int Axis, CString Time, CString CStrPos)
{
	CString CstrAxis, CStrCurTime;
	switch (Axis)
	{
	case X:
		CstrAxis = _T("X축");
		break;
	case Y:
		CstrAxis = _T("Y축");
		break;
	case Z:
		CstrAxis = _T("Z축");
		break;
	}

	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	CString filePath, StrPosition, directoryPath;
	CStdioFile File;
	BOOL bOpen;

	setlocale(LC_ALL, "Korean");
	directoryPath.Format(_T("../Log\\%4d년"), CurrentTime.wYear);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월"), CurrentTime.wYear, CurrentTime.wMonth);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	CreateDirectory(directoryPath, NULL);

	filePath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\Position.txt"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	bOpen = File.Open(filePath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite);
	if (!bOpen)
	{
		SavePosLogFile(Axis, Time, CStrPos);
		//AfxMessageBox(_T("LOG File Create/Open Error"));
	}
	else
	{
		CStrCurTime = pAJinLibrarySeq->StrCurrentTime();
		File.SeekToEnd();
		StrPosition.Format(_T("%s| %s소요 | %f %s \n"), CStrCurTime, CstrAxis, Time, CStrPos);
		File.WriteString(StrPosition);
		File.Close();
	}
	pMainDlg->pLogDlg->AddPosLog(Axis, Time, CStrPos, CStrCurTime);
}


void SequenceDlg::SavePosLogFile(CString str)
{
	CString CstrAxis;

	CString filePath, StrPosition, directoryPath;
	CStdioFile File;
	BOOL bOpen;
	CString strFin, CStrCurTime;
	CStrCurTime = pAJinLibrarySeq->StrCurrentTime();
	CurrentTime = pAJinLibrarySeq->TCurrentTime();
	setlocale(LC_ALL, "Korean");
	directoryPath.Format(_T("../Log\\%4d년"), CurrentTime.wYear);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월"), CurrentTime.wYear, CurrentTime.wMonth);
	CreateDirectory(directoryPath, NULL);
	directoryPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	CreateDirectory(directoryPath, NULL);

	filePath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\Position.txt"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
	bOpen = File.Open(filePath, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite);
	if (!bOpen)
	{
		SavePosLogFile(str);
		//AfxMessageBox(_T("LOG File Create/Open Error"));
	}
	else
	{
		if (str == _T("Initialize 시작"))
		{
			TInitStart = CurrentTime;
		}
		else if (str == _T("Initialize 완료"))
		{
			TInitEnd = CurrentTime;
		}
		else if (str == _T("AutoRun 시작"))
		{
			TAutoStart = CurrentTime;
		}
		else if (str == _T("AutoRun 완료"))
		{
			TAutoEnd = CurrentTime;
		}

		TInitTime = pAJinLibrarySeq->CompareSystemTime(TInitEnd, TInitStart);
		TAutoTime = pAJinLibrarySeq->CompareSystemTime(TAutoEnd, TAutoStart);

		if (str == _T("Initialize 완료"))
		{
			strFin = pAJinLibrarySeq->GetStrHowLong(TInitTime, _T("Initialize")); 
			File.SeekToEnd();
			File.WriteString(CStrCurTime);
			File.WriteString(_T("| "));
			File.WriteString(str);
			File.WriteString(strFin);
			File.WriteString(_T("\n"));
			File.Close();
		}
		else if (str == _T("AutoRun 완료"))
		{
			strFin = pAJinLibrarySeq->GetStrHowLong(TAutoTime, _T("AutoRun"));
			File.SeekToEnd();
			File.WriteString(CStrCurTime);
			File.WriteString(_T("| "));
			File.WriteString(str);
			File.WriteString(strFin);
			File.WriteString(_T("\n"));
			File.Close();
		}
		else
		{
			File.SeekToEnd();
			File.WriteString(CStrCurTime);
			File.WriteString(_T("| "));
			File.WriteString(str);
			File.WriteString(_T("\n"));
			File.Close();
		}
	}
	pMainDlg->pLogDlg->AddPosLog(str, CStrCurTime);
}

UINT SequenceDlg::AutoRunAlarmThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (true)
	{
		BOOL b3OutputFlag = pIOClassSeq->GetOutput(1, 3);
		BOOL b4OutputFlag = pIOClassSeq->GetOutput(1, 4);
		BOOL b5OutputFlag = pIOClassSeq->GetOutput(1, 5);

		if (b3OutputFlag || b4OutputFlag || b5OutputFlag)
		{
			if (b3OutputFlag)
				AfxMessageBox(_T("1번 모듈의 3번 OUTPUT ON!"));
			if (b4OutputFlag)
				AfxMessageBox(_T("1번 모듈의 4번 OUTPUT ON!"));
			if (b5OutputFlag)
				AfxMessageBox(_T("1번 모듈의 5번 OUTPUT ON!"));

			pIOClassSeq->SetOutput(0, TOWERLAMPRED, TRUE);
		}
		else
		{
			pIOClassSeq->SetOutput(0, TOWERLAMPRED, FALSE);
		}

		Sleep(5000);
	}

		
	return 0;
}

UINT SequenceDlg::AutoRunBtnColorThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (true)
	{

		if (Sequence->IsAutoRun)
			Sequence->m_AutoRunBtn.SetFaceColor(RGB(0, 255, 0));
		else
		{
			Sequence->m_AutoRunBtn.SetFaceColor(RGB(150, 150, 150));
		}
		Sleep(10);
	}

	return 0;
}

void SequenceDlg::TowerBlinkStart(int TowerColor)
{
	switch (TowerColor)
	{
	case TOWERLAMPRED:
		m_pTowrRedBlinkThread = AfxBeginThread(TowerRedBlinkThread, this);
		if (m_pTowrRedBlinkThread == NULL)
			AfxMessageBox(_T("Error!"));
		break;
	case TOWERLAMPGREEN:
		m_ptowerGreenBlinkThread = AfxBeginThread(TowerGreenBlinkThread, this);
		if (m_ptowerGreenBlinkThread == NULL)
			AfxMessageBox(_T("Error!"));
		break;
	default:
		break;
	}
}

UINT SequenceDlg::TowerRedBlinkThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (Sequence->IsInit == FALSE)
	{
		pIOClassSeq->SetOutput(0, TOWERLAMPRED, TRUE);
		Sleep(1000);
		pIOClassSeq->SetOutput(0, TOWERLAMPRED, FALSE);
		Sleep(1000);
	}
	if (Sequence->IsInit == TRUE)
	{
		return 0;
	}
	return 0;
}

UINT SequenceDlg::TowerGreenBlinkThread(LPVOID _mothod)
{
	SequenceDlg *Sequence = (SequenceDlg*)_mothod;
	while (Sequence->IsInit == TRUE)
	{
		pIOClassSeq->SetOutput(0, TOWERLAMPGREEN, TRUE);
		Sleep(1000);
		pIOClassSeq->SetOutput(0, TOWERLAMPGREEN, FALSE);
		Sleep(1000);
		if (Sequence->IsAutoRun == TRUE)
		{
			return 0;
		}
	}
	return 0;
}

#pragma region stop thread : ondestroy 이벤트
void SequenceDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pStopThread)
	{
		::SuspendThread(m_pStopThread->m_hThread);
	}
}
#pragma endregion

void SequenceDlg::OnBnClickedApplyBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Position 값 저장 및 적용 버튼 클릭 이벤트 발생"));
	UpdatePosInfo();

	string filePath = "../Config\\PosInfo.txt";

	// write File
	ofstream writeFile(filePath.data());
	if (writeFile.is_open())
	{
		writeFile << "Motor Waiting Time : " << dWaitTimeSec << "\n";

		writeFile << "Axis X Ready Position : " << dReadyPosX << "\n";
		writeFile << "Axis X Work Ready Position : " << dWorkReadyPosX << "\n";		
		writeFile << "Axis X Working Position #1 : " << dWorkPosX_1 << "\n";		
		writeFile << "Axis X Working Position #2 : " << dWorkPosX_2 << "\n";		
		writeFile << "Axis X Working Position #3 : " << dWorkPosX_3 << "\n" << "\n";
		
		writeFile << "Axis Y Ready Position : " << dReadyPosY << "\n";		
		writeFile << "Axis Y Work Ready Position : " << dWorkReadyPosY << "\n";
		writeFile << "Axis Y Working Position #1 : " << dWorkPosY_1 << "\n";
		writeFile << "Axis Y Working Position #2 : " << dWorkPosY_2 << "\n";
		writeFile << "Axis Y Working Position #3 : " << dWorkPosY_3 << "\n" << "\n";

		writeFile << "Axis Z Ready Position : " << dReadyPosZ << "\n";
		writeFile << "Axis Z Work Ready Position : " << dWorkReadyPosZ << "\n";
		writeFile << "Axis Z Working Position #1 : " << dWorkPosZ_1 << "\n";
		writeFile << "Axis Z Working Position #2 : " << dWorkPosZ_2 << "\n";
		writeFile << "Axis Z Working Position #3 : " << dWorkPosZ_3 << "\n";
		writeFile.close();
	}
}

void SequenceDlg::UpdatePosInfo()
{
	dWaitTimeSec = GetDlgItemDouble(IDC_EDT_WAITTIME);
	dWaitTimeMS = (DWORD)dWaitTimeSec * 1000;
	dReadyPosX = GetDlgItemDouble(IDC_EDT_XREADYPOS);
	dWorkReadyPosX = GetDlgItemDouble(IDC_EDT_XWRPOS);
	dWorkPosX_1 = GetDlgItemDouble(IDC_EDT_XWORKPOS1);
	dWorkPosX_2 = GetDlgItemDouble(IDC_EDT_XWORKPOS2);
	dWorkPosX_3 = GetDlgItemDouble(IDC_EDT_XWORKPOS3);

	dReadyPosY = GetDlgItemDouble(IDC_EDT_YREADYPOS);
	dWorkReadyPosY = GetDlgItemDouble(IDC_EDT_YWRPOS);
	dWorkPosY_1 = GetDlgItemDouble(IDC_EDT_YWORKPOS1);
	dWorkPosY_2 = GetDlgItemDouble(IDC_EDT_YWORKPOS2);
	dWorkPosY_3 = GetDlgItemDouble(IDC_EDT_YWORKPOS3);

	dReadyPosZ = GetDlgItemDouble(IDC_EDT_ZREADYPOS);
	dWorkReadyPosZ = GetDlgItemDouble(IDC_EDT_ZWRPOS);
	dWorkPosZ_1 = GetDlgItemDouble(IDC_EDT_ZWORKPOS1);
	dWorkPosZ_2 = GetDlgItemDouble(IDC_EDT_ZWORKPOS2);
	dWorkPosZ_3 = GetDlgItemDouble(IDC_EDT_ZWORKPOS3);

	if (pAJinLibrarySeq->IsLibraryOpen())
	{
		pMotionClassSeq->SetAbsMode(X);
		pMotionClassSeq->SetAbsMode(Y);
		pMotionClassSeq->SetAbsMode(Z);
	}
}

void SequenceDlg::OnBnClickedLoadBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Position 값 불러오기 버튼 클릭 이벤트 발생"));
	LoadPosInfo();
}

void SequenceDlg::LoadPosInfo()
{
	CString strData;
	// read File	
	CString filePath = _T("../Config\\PosInfo.txt");
	CStdioFile file;
	vector<CString> vstrPosInfo;
	file.Open(filePath, CStdioFile::modeRead);  //file을 열고 읽음

	CString strTmp;

	while (file.ReadString(strTmp))
	{
		for (int i = 0; i < strTmp.GetLength(); i++)
		{
			if (string(":").find((char)strTmp[i]) == -1)
				continue;
			string str = CT2CA(strTmp);
			str = str.substr(i + 1, strTmp.GetLength());
			strTmp = str.c_str();
			vstrPosInfo.push_back(strTmp);
		}
	}

	file.Close();
	dWaitTimeSec = _wtof(vstrPosInfo[0]);
	dReadyPosX = _wtof(vstrPosInfo[1]);
	dWorkReadyPosX = _wtof(vstrPosInfo[2]);
	dWorkPosX_1 = _wtof(vstrPosInfo[3]);
	dWorkPosX_2 = _wtof(vstrPosInfo[4]);
	dWorkPosX_3 = _wtof(vstrPosInfo[5]);
	dReadyPosY = _wtof(vstrPosInfo[6]);
	dWorkReadyPosY = _wtof(vstrPosInfo[7]);
	dWorkPosY_1 = _wtof(vstrPosInfo[8]);
	dWorkPosY_2 = _wtof(vstrPosInfo[9]);
	dWorkPosY_3 = _wtof(vstrPosInfo[10]);
	dReadyPosZ = _wtof(vstrPosInfo[11]);
	dWorkReadyPosZ = _wtof(vstrPosInfo[12]);
	dWorkPosZ_1 = _wtof(vstrPosInfo[13]);
	dWorkPosZ_2 = _wtof(vstrPosInfo[14]);
	dWorkPosZ_3 = _wtof(vstrPosInfo[15]);

	SetDlgItemDouble(IDC_EDT_WAITTIME, dWaitTimeSec);
	SetDlgItemDouble(IDC_EDT_XREADYPOS, dReadyPosX);
	SetDlgItemDouble(IDC_EDT_XWRPOS, dWorkReadyPosX);
	SetDlgItemDouble(IDC_EDT_XWORKPOS1, dWorkPosX_1);
	SetDlgItemDouble(IDC_EDT_XWORKPOS2, dWorkPosX_2);
	SetDlgItemDouble(IDC_EDT_XWORKPOS3, dWorkPosX_3);

	SetDlgItemDouble(IDC_EDT_YREADYPOS, dReadyPosY);
	SetDlgItemDouble(IDC_EDT_YWRPOS, dWorkReadyPosY);
	SetDlgItemDouble(IDC_EDT_YWORKPOS1, dWorkPosY_1);
	SetDlgItemDouble(IDC_EDT_YWORKPOS2, dWorkPosY_2);
	SetDlgItemDouble(IDC_EDT_YWORKPOS3, dWorkPosY_3);

	SetDlgItemDouble(IDC_EDT_ZREADYPOS, dReadyPosZ);
	SetDlgItemDouble(IDC_EDT_ZWRPOS, dWorkReadyPosZ);
	SetDlgItemDouble(IDC_EDT_ZWORKPOS1, dWorkPosZ_1);
	SetDlgItemDouble(IDC_EDT_ZWORKPOS2, dWorkPosZ_2);
	SetDlgItemDouble(IDC_EDT_ZWORKPOS3, dWorkPosZ_3);
}

#pragma region Get/Set DlgitemDouble
void SequenceDlg::SetDlgItemDouble(int nID, double value, int nPoint)
{
	CString sTemp, sTemp2;
	sTemp2.Format(_T("%%.%df"), nPoint);
	sTemp.Format(sTemp2, value);
	GetDlgItem(nID)->SetWindowText(sTemp);
}
double SequenceDlg::GetDlgItemDouble(int nID)
{
	double dRet;
	CString sTemp;
	GetDlgItem(nID)->GetWindowText(sTemp);
	dRet = _wtof(sTemp);
	return dRet;
}
#pragma endregion

#pragma region X singleMotion
void SequenceDlg::OnBnClickedReadyxmoveBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - X축 Ready 위치 이동 버튼 클릭 이벤트 발생"));
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(X, dReadyPosX, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(X, dReadyPosX , _T("(Ready Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedWrxmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - X축 Working Ready 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(X, dWorkReadyPosX, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(X, dWorkReadyPosX, _T("(Work Ready Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW1xmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - X축 Working 1 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
		double	dMoveVel, dMoveAcc, dMoveDec;
		pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
		pMotionClassSeq->SingleMove(X, dWorkPosX_1, dMoveVel, dMoveAcc, dMoveDec);
		SavePosLogFile(X, dWorkPosX_1, _T("(Working #1 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW2xmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - X축 Working 2 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(X, dWorkPosX_2, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(X, dWorkPosX_2, _T("(Working #2 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW3xmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - X축 Working 3 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(X, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(X, dWorkPosX_3, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(X, dWorkPosX_3, _T("(Working #3 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}
#pragma endregion

#pragma  region Y singleMotion
void SequenceDlg::OnClickedReadyymoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Y축 Ready 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag){
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Y, dReadyPosY, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Y, dReadyPosY, _T("(Ready Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedWrymoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Y축 Working Ready 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Y, dWorkReadyPosY, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Y, dWorkReadyPosY, _T("(Work Ready Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW1ymoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Y축 Working 1 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Y, dWorkPosY_1, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Y, dWorkPosY_1, _T("(Working #1 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW2ymoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Y축 Working 2 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Y, dWorkPosY_2, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Y, dWorkPosY_2, _T("(Working #2 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW3ymoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Y축 Working 3 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Y, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Y, dWorkPosY_3, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Y, dWorkPosY_3, _T("(Working #3 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}
#pragma endregion

#pragma  region Z singleMotion
void SequenceDlg::OnBnClickedReadyzmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Z축 Ready 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Z, dReadyPosZ, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Z, dReadyPosZ, _T("(Ready Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedWrzmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Z축 Working Ready 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Z, dWorkReadyPosZ, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Z, dWorkReadyPosZ, _T("(Work Ready Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}

void SequenceDlg::OnBnClickedW1zmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Z축 Working 1 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Z, dWorkPosZ_1, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Z, dWorkPosZ_1, _T("(Working #1 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW2zmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Z축 Working 2 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
			double	dMoveVel, dMoveAcc, dMoveDec;
			pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
			pMotionClassSeq->SingleMove(Z, dWorkPosZ_2, dMoveVel, dMoveAcc, dMoveDec);
			SavePosLogFile(Z, dWorkPosZ_2, _T("(Working #2 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}


void SequenceDlg::OnBnClickedW3zmoveBtn()
{
	pAJinLibrarySeq->SaveActionLogFile(_T("Sequence Dlg - Z축 Working 3 위치 이동 버튼 클릭 이벤트 발생"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bEMOFlag)
	{
		if (IsInit)
		{
		double	dMoveVel, dMoveAcc, dMoveDec;
		pSettingClassSeq->GetParameter(Z, NULL, &dMoveVel, &dMoveAcc, &dMoveDec);
		pMotionClassSeq->SingleMove(Z, dWorkPosZ_3, dMoveVel, dMoveAcc, dMoveDec);
		SavePosLogFile(Z, dWorkPosZ_3, _T("(Working #3 Position)"));
		}
		else
		{
			AfxMessageBox(_T("Initialize 완료 후 시도해주세요!"));
		}
	}
	else
	{
		AfxMessageBox(_T("EMO가 눌려있습니다! EMO를 끄고 다시 시도해주세요!"));
	}
}
#pragma endregion