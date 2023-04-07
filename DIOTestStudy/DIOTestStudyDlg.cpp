
// DIOTestStudyDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "DIOTestStudyDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDIOTestStudyDlg 대화 상자

#define	TM_SENSOR_INTERVAL	10


#ifdef _M_X64
#pragma comment(lib,  "../AXL(Library)/Library/64Bit/AxL.lib")
#else
#pragma comment(lib,  "../AXL(Library)/Library/32Bit/AxL.lib")
#endif


CDIOTestStudyDlg::CDIOTestStudyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDIOTestStudyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDIOTestStudyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDRETRY, m_Tab);
	DDX_Control(pDX, IDC_CONNSTRING, m_connstring);
}

BEGIN_MESSAGE_MAP(CDIOTestStudyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDRETRY, &CDIOTestStudyDlg::OnTcnSelchangeRetry)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CDIOTestStudyDlg::OnBnClickedButton1)
	
	ON_WM_DESTROY()
	ON_BN_CLICKED(CONNBTN, &CDIOTestStudyDlg::OnBnClickedConnbtn)
	ON_BN_CLICKED(IDC_BtnSq, &CDIOTestStudyDlg::OnBnClickedBtnsq)
END_MESSAGE_MAP()


// CDIOTestStudyDlg 메시지 처리기

BOOL CDIOTestStudyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	pIOClass = new CIOClass;
	pAJLClass = new CAJinLibrary;
	pMotionClass = new CMotionClass;
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	pAJLClass->LibraryOpen();
	TabControlItem();

	if (AXTInit())
	{
		CWnd* connbtn = GetDlgItem(CONNBTN);
		connbtn->EnableWindow(FALSE);
		m_pThread = AfxBeginThread(TimeThread, this);
		if (m_pThread == NULL)
			AfxMessageBox(_T("Error!"));
	}

	pSequenceDlg = new SequenceDlg;
	pSequenceDlg->Create(IDD_SEQUENCE_DIALOG);

	pMotionClass->SetHomeResult(0);
	pMotionClass->SetHomeResult(1);
	pMotionClass->SetHomeResult(2);

	pAJLClass->SaveActionLogFile(_T("Main Dlg - 프로그램시작 이벤트 발생"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.

}

 //대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
 // 아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
 // 프레임워크에서 이 작업을 자동으로 수행합니다.

void CDIOTestStudyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.

HCURSOR CDIOTestStudyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#pragma region AXTInit
BOOL CDIOTestStudyDlg::AXTInit()
{
	// Initialize library 	
	if (pAJLClass->IsLibraryOpen())
	{
		TRACE(_T("Library is initialized .\n"));
		
		// Inspect if DIO module exsits
		if (pIOClass->ISDioModuleExist())
		{
			TRACE(_T("DIO module exists.\n"));
			SetDlgItemText(IDC_CONNSTRING, _T("connected"));
			pAJLClass->IsGetModuleCount();
		}
		else
		{
			AfxMessageBox(_T("Module not exist."));
			SetDlgItemText(IDC_CONNSTRING, _T("disconnected"));
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("Failed initialization."));
		SetDlgItemText(IDC_CONNSTRING, _T("disconnected"));
		return FALSE;
	}

	return TRUE;
}
#pragma endregion

#pragma region 탭 컨트롤 아이템 추가 함수
void CDIOTestStudyDlg::TabControlItem()
{

	m_Tab.InsertItem(0, _T("IO"));
	m_Tab.InsertItem(1, _T("MOTION"));
	m_Tab.InsertItem(2, _T("LOG"));
	m_Tab.SetCurSel(0);

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	pDlg1 = new IO;
	pDlg1->Create(IDD_DIALOG1, &m_Tab);
	pDlg1->MoveWindow(0, 25, rect.Width(), rect.Height());
	pDlg1->ShowWindow(SW_SHOW);

	pMotionDlg = new MOTION;
	pMotionDlg->Create(IDD_MOTION_DIALOG, &m_Tab);
	pMotionDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	pMotionDlg->ShowWindow(SW_HIDE);

	pLogDlg = new CLogDlg;
	pLogDlg->Create(IDD_LOG_DIALOG, &m_Tab);
	pLogDlg->MoveWindow(0, 25, rect.Width(), rect.Height());
	pLogDlg->ShowWindow(SW_HIDE);
}
#pragma endregion

#pragma region 탭 컨트롤 selchange함수
void CDIOTestStudyDlg::OnTcnSelchangeRetry(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		pDlg1->ShowWindow(SW_SHOW);
		pMotionDlg->ShowWindow(SW_HIDE);
		pLogDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		pDlg1->ShowWindow(SW_HIDE);
		pMotionDlg->ShowWindow(SW_SHOW);
		pLogDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		pDlg1->ShowWindow(SW_HIDE);
		pMotionDlg->ShowWindow(SW_HIDE);
		pLogDlg->ShowWindow(SW_SHOW);
		break;

	}
	
	*pResult = 0;

}
#pragma endregion

#pragma region 창 닫을 때 library, motion exit, 타이머종료
//닫기 버튼 클릭이벤트
void CDIOTestStudyDlg::OnBnClickedButton1()
{
	CloseDevice();

	OnOK();
}

void CDIOTestStudyDlg::OnDestroy()
{
	CDialog::OnDestroy();

	pAJLClass->SaveActionLogFile(_T("Main Dlg - 프로그램종료 이벤트 발생"));

	if (NULL != m_pThread) 
	{
		::SuspendThread(m_pThread->m_hThread);
	}
	CloseDevice();
}

BOOL CDIOTestStudyDlg::CloseDevice()
{
	// Verify if the integrated library is available to use (If it is initialized)  
	return	pAJLClass->LibraryClose();

	SetDlgItemText(IDC_CONNSTRING, _T("disconnected"));	
}

#pragma endregion

#pragma region 모듈 연결 확인을 위한 connection state string color ctrl 
HBRUSH CDIOTestStudyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_CONNSTRING) 
	{
		CString connstat;
		GetDlgItemText(IDC_CONNSTRING, connstat);
		if (connstat == "connected")
		{
			pDC->SetTextColor(RGB(0, 150, 0)); 
		}
		else if (connstat == "disconnected")
		{
			pDC->SetTextColor(RGB(150, 0, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(150, 150, 150));
		}
	}

	return hbr;
}
#pragma endregion

#pragma region 연결 안되었을 때 재접속 버튼 클릭 이벤트
void CDIOTestStudyDlg::OnBnClickedConnbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pAJLClass->SaveActionLogFile(_T("Main Dlg - 재접속 시도 버튼 클릭 이벤트 발생"));
	if (AXTInit())
	{
		CWnd* connbtn = GetDlgItem(CONNBTN);
		connbtn->EnableWindow(FALSE);		
		m_pThread = AfxBeginThread(TimeThread, this);
		if (m_pThread == NULL)
			AfxMessageBox(_T("Error!"));
		SetDlgItemText(IDC_CONNSTRING, _T("connected"));
	}
	else
	{
		AfxMessageBox(_T("재접속 실패!"));
		SetDlgItemText(IDC_CONNSTRING, _T("disconnected"));
	}
}
#pragma endregion

#pragma region IO확인
UINT CDIOTestStudyDlg::TimeThread(LPVOID _mothod)
{
	CDIOTestStudyDlg *fir = (CDIOTestStudyDlg*)_mothod;

	short		nIndex;
	DWORD		dwDataInput;
	DWORD		dwDataOutput;
	long        IModuleCounts;
	CString str;
	while (1)
	{
		AxdInfoGetModuleCount(&IModuleCounts);

		if (IModuleCounts > 0)
		{
			//Input I/O 상태업데이트
			for (nIndex = 0; nIndex < fir->pDlg1->IInputCounts; nIndex++)
			{
				dwDataInput = fir->pIOClass->GetInput(fir->pDlg1->ModuleNo, nIndex);
				fir->pDlg1->m_pchkInput[nIndex]->SetCheck(dwDataInput);
				if (dwDataInput)
				{
					fir->pDlg1->m_pchkInput[nIndex]->SetWindowText(_T("ON"));
				}
				else
				{
					fir->pDlg1->m_pchkInput[nIndex]->SetWindowText(_T("OFF"));
				}
			}

			//Output I/O 상태업데이트
			for (nIndex = 0; nIndex < fir->pDlg1->IOutputCounts; nIndex++)
			{
				dwDataOutput = fir->pIOClass->GetOutput(fir->pDlg1->ModuleNo, nIndex);
				fir->pDlg1->m_pchkOutput[nIndex]->SetCheck(dwDataOutput);
				if (dwDataOutput)
				{
					fir->pDlg1->m_pchkOutput[nIndex]->SetWindowText(_T("ON"));
				}
				else
				{
					fir->pDlg1->m_pchkOutput[nIndex]->SetWindowText(_T("OFF"));
				}
			}

		}
		Sleep(TM_SENSOR_INTERVAL);
	}
	return 0;
}
#pragma endregion

void CDIOTestStudyDlg::OnBnClickedBtnsq()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!pSequenceDlg->bflag)
	{
		pSequenceDlg->ShowWindow(SW_SHOW);
		pMotionClass->SetHomeResult(0);
		pMotionClass->SetHomeResult(1);
		pMotionClass->SetHomeResult(2);
		pSequenceDlg->bflag = TRUE;
	}
	else
	{
		pSequenceDlg->ShowWindow(SW_HIDE);
		pSequenceDlg->bflag = FALSE;
	}
}
