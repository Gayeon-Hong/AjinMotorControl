// STATUS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "STATUS.h"
#include "afxdialogex.h"
#include "MOTION.h"
#include "AJinLibrary.h"

#define	TM_SENSOR_ID		1000
#define	TM_SENSOR_INTERVAL	10
// STATUS 대화 상자입니다.

IMPLEMENT_DYNAMIC(STATUS, CDialogEx)
CMotionClass  *pMotionClass;
CStatusClass  *pStatusClass;
CAJinLibrary  *pAJinLibraryStatus;
STATUS::STATUS(CWnd* pParent /*=NULL*/)
	: CDialogEx(STATUS::IDD, pParent)
{

}

STATUS::~STATUS()
{
}

void STATUS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(STATUS, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &STATUS::OnBnClickedButton1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// STATUS 메시지 처리기입니다.
BOOL STATUS::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pStatusClass = new CStatusClass;
	pMotionClass = new CMotionClass;
	pAJinLibraryStatus = new CAJinLibrary;
	InitStatus();
	m_pThread = AfxBeginThread(StatusThread, this);
		if (m_pThread == NULL)
			AfxMessageBox(_T("Error!"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#pragma region 배열에 컨트롤 등록, Init Status
void STATUS::InitStatus()
{
	m_lAxisCount = pMotionClass->GetAxisCount();
	if (m_lAxisCount > 0)
	{
		//STATUS 초기화
		for (int i = 0; i < m_lAxisCount; i++)
		{
			m_ServoOn[i] = (CMFCButton *)GetDlgItem(IDC_ServoOn00 + i);
			m_Alarm[i] = (CMFCButton *)GetDlgItem(IDC_Alarm00 + i);
			m_InMotion[i] = (CMFCButton *)GetDlgItem(IDC_INMOTION00 + i);
			m_InPosition[i] = (CMFCButton *)GetDlgItem(IDC_INPOSITION00 + i);
			m_HOME[i] = (CMFCButton *)GetDlgItem(IDC_HOME00 + i);
			m_PELIM[i] = (CMFCButton *)GetDlgItem(IDC_PELIM00 + i);
			m_MELIM[i] = (CMFCButton *)GetDlgItem(IDC_MELIM00 + i);
			m_EMG[i] = (CMFCButton *)GetDlgItem(IDC_EMG00 + i);
			m_HomeSearch[i] = (CMFCButton *)GetDlgItem(IDC_HOMESEARCH00 + i);

			bSVObtnFlag[i] = pStatusClass->GetServoOnOFF(i);
			bAbtnFlag[i] = pStatusClass->GetAlarmOnOFF(i);
			bInMotionFlag[i] = pStatusClass->GetInMotionHL(i);
			bInPositionFlag[i] = pStatusClass->GetInPositionHL(i);
			bHOMEFlag[i] = pStatusClass->GetHomeOnOFF(i);
			bPELIMFlag[i] = pStatusClass->GetPElimitHL(i);
			bMELIMFlag[i] = pStatusClass->GetMElimitHL(i);
			bEMGFlag[i] = pStatusClass->GetEMGHL(i);
		}
		m_pThread = AfxBeginThread(StatusThread, this);
		if (m_pThread == NULL)
			AfxMessageBox(_T("Error!"));
	}
}
#pragma endregion 

#pragma region 닫기 버튼 이벤트
void STATUS::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
	bflag = FALSE;
}
#pragma endregion

UINT STATUS::StatusThread(LPVOID _mothod)
{
	STATUS *stat = (STATUS*)_mothod;

	while (true)
	{
		for (int i = 0; i < stat->m_lAxisCount; i++)
		{
			if (pStatusClass->GetServoOnOFF(i) == HIGH)
			{
				stat->m_ServoOn[i]->SetFaceColor(RGB(0, 255, 0));
			}
			else if (pStatusClass->GetServoOnOFF(i) == LOW)
			{
				pMotionClass->SetHomeResult(i);
				stat->m_ServoOn[i]->SetFaceColor(RGB(255, 0, 0));
			}

			if (pStatusClass->GetAlarmOnOFF(i) == HIGH)
			{
				pMotionClass->SetHomeResult(i);
				stat->m_Alarm[i]->SetFaceColor(RGB(255, 0, 0));
			}
			else if (pStatusClass->GetAlarmOnOFF(i) == LOW)
			{
				stat->m_Alarm[i]->SetFaceColor(RGB(150, 150, 150));
			}
			else
			{
				stat->m_Alarm[i]->SetFaceColor(RGB(150, 150, 150));
			}

			stat->bInMotionFlag[i] = pStatusClass->GetInMotionHL(i);
			if (stat->bInMotionFlag[i] == HIGH)
			{
				stat->m_InMotion[i]->SetFaceColor(RGB(0, 255, 0));
			}
			else if (stat->bInMotionFlag[i] == LOW)
			{
				stat->m_InMotion[i]->SetFaceColor(RGB(255, 0, 0));
			}

			if (!pStatusClass->GetInPositionIsUse(i))
			{
				stat->m_InPosition[i]->SetFaceColor(RGB(100, 100, 100));
			}
			else
			{
				stat->bInPositionFlag[i] = pStatusClass->GetInPositionHL(i);
				if (stat->bInPositionFlag[i] == HIGH)
				{
					stat->m_InPosition[i]->SetFaceColor(RGB(0, 255, 0));
				}
				else if (stat->bInPositionFlag[i] == LOW)
				{
					stat->m_InPosition[i]->SetFaceColor(RGB(255, 0, 0));
				}
			}

			stat->bHOMEFlag[i] = pStatusClass->GetHomeOnOFF(i);
			if (stat->bHOMEFlag[i] == TRUE)
			{
				stat->m_HOME[i]->SetFaceColor(RGB(0, 255, 0));
			}
			else
			{
				stat->m_HOME[i]->SetFaceColor(RGB(255, 0, 0));
			}

			stat->bPELIMFlag[i] = pStatusClass->GetPElimitHL(i);
			if (stat->bPELIMFlag[i] == UNUSED)
			{
				stat->m_PELIM[i]->SetFaceColor(RGB(100, 100, 100));
			}
			else
			{
				if (stat->bPELIMFlag[i] == HIGH)
				{
					stat->m_PELIM[i]->SetFaceColor(RGB(255, 0, 0));
				}
				else if (stat->bPELIMFlag[i] == LOW)
				{
					stat->m_PELIM[i]->SetFaceColor(RGB(150, 150, 150));
				}

			}


			stat->bMELIMFlag[i] = pStatusClass->GetMElimitHL(i);
			if (stat->bMELIMFlag[i] == UNUSED)
			{
				stat->m_MELIM[i]->SetFaceColor(RGB(100, 100, 100));
			}
			else
			{
				if (stat->bMELIMFlag[i] == TRUE)
				{
					stat->m_MELIM[i]->SetFaceColor(RGB(255, 0, 0));
				}
				else
				{
					stat->m_MELIM[i]->SetFaceColor(RGB(150, 150, 150));
				}
			}

			stat->bEMGFlag[i] = pStatusClass->GetEMGHL(i);
			if (stat->bEMGFlag[i] == UNUSED)
			{
				stat->m_EMG[i]->SetFaceColor(RGB(100, 100, 100));
			}
			else
			{
				if (stat->bEMGFlag[i] == HIGH)
				{
					stat->m_EMG[i]->SetFaceColor(RGB(255, 0, 0));
				}
				else if (stat->bEMGFlag[i] == LOW)
				{
					stat->m_EMG[i]->SetFaceColor(RGB(150, 150, 150));
				}
			}

			if (pStatusClass->GetHomeSearchResult(i))
			{
				stat->m_HomeSearch[i]->SetFaceColor(RGB(0, 255, 0));
			}
			else{
				stat->m_HomeSearch[i]->SetFaceColor(RGB(255, 0, 0));
			}
		}
		Sleep(TM_SENSOR_INTERVAL);
	}
	return 0;
}
void STATUS::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pThread) {
		::SuspendThread(m_pThread->m_hThread);
	}
}