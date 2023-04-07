// LogDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "LogDlg.h"
#include "afxdialogex.h"
#include "AJinLibrary.h"
#include "DIOTestStudyDlg.h"

// CLogDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogDlg, CDialogEx)
CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogDlg::IDD, pParent)
{
}

CLogDlg::~CLogDlg()
{
}

CAJinLibrary *pAjinLibraryLog;
CDIOTestStudyDlg *pDIOTestLog;

BOOL CLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pDIOTestLog = (CDIOTestStudyDlg *)AfxGetMainWnd();
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	pAjinLibraryLog = new CAJinLibrary;

	AddPosItem();
	AddActionItem();
	
	LoadTodayPosLog();
	LoadTodayActionLog();

	return TRUE;  
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POS_LIST, m_PosLogList);
	DDX_Control(pDX, IDC_ACTION_LIST, m_ActionLogList);
	DDX_Control(pDX, IDC_POS_SEL_LIST, m_PosSelLogList);
	DDX_Control(pDX, IDC_ACTION_SEL_LIST, m_ActionSelLogList);
	DDX_Control(pDX, IDC_POS_DATETIMEPICKER, m_dateTimePos);
	DDX_Control(pDX, IDC_ACTION_DATETIMEPICKER, m_dateTimeAction);
	DDX_Control(pDX, IDC_POS_COMBO, m_PosCombo);
	DDX_Control(pDX, IDC_ACTION_COMBO, m_ActionCombo);
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_POS_DATETIMEPICKER, &CLogDlg::OnDtnDatetimechangePosDatetimepicker)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_ACTION_DATETIMEPICKER, &CLogDlg::OnDtnDatetimechangeActionDatetimepicker)
	ON_CBN_SELCHANGE(IDC_POS_COMBO, &CLogDlg::OnCbnSelchangePosCombo)
	ON_CBN_SELCHANGE(IDC_ACTION_COMBO, &CLogDlg::OnCbnSelchangeActionCombo)
END_MESSAGE_MAP()


// CLogDlg 메시지 처리기입니다.



void  CLogDlg::AddPosItem()
{
	m_PosCombo.AddString(_T("ALL"));
	m_PosCombo.AddString(_T("X축"));
	m_PosCombo.AddString(_T("Y축"));
	m_PosCombo.AddString(_T("Z축"));
	m_PosCombo.AddString(_T("READY 위치"));
	m_PosCombo.AddString(_T("WORK READY 위치"));
	m_PosCombo.AddString(_T("WORKING #1 위치"));
	m_PosCombo.AddString(_T("WORKING #2 위치"));
	m_PosCombo.AddString(_T("WORKING #3 위치"));
	m_PosCombo.AddString(_T("INITIALIZE (시작 - 종료)"));
	m_PosCombo.AddString(_T("AUTORUN (시작 - 종료)"));
	PosFilter = ALL;
	m_PosCombo.SetCurSel(PosFilter);
}

void  CLogDlg::AddActionItem()
{
	m_ActionCombo.AddString(_T("ALL"));
	m_ActionCombo.AddString(_T("프로그램 시작"));
	m_ActionCombo.AddString(_T("프로그램 종료"));
	m_ActionCombo.AddString(_T("OUTPUT ON"));
	m_ActionCombo.AddString(_T("OUTPUT OFF"));
	m_ActionCombo.AddString(_T("I/O DIALOG"));
	m_ActionCombo.AddString(_T("SEQUENCE DIALOG"));
	m_ActionCombo.AddString(_T("PARAMETER SETTING DIALOG"));
	m_ActionCombo.AddString(_T("MAIN DIALOG"));
	m_ActionCombo.AddString(_T("MOTION DIALOG"));
	ActionFilter = ALL;
	m_ActionCombo.SetCurSel(ActionFilter);
}

void CLogDlg::AddPosLog(CString str, CString CStrCurTime)
{
	CString strTmp, strFin;

	strTmp.Format(_T("%s| %s"), CStrCurTime, str);
	m_PosLogList.InsertString(0, strTmp);
	if (IsPosToday())
		m_PosSelLogList.InsertString(0, strTmp);

	if (str == _T("Initialize 완료"))
	{
		strFin = pAjinLibraryLog->GetStrHowLong(pDIOTestLog->pSequenceDlg->TInitTime, _T("Initialize"));
		m_PosLogList.InsertString(0, strFin);
		if (IsPosToday())
			m_PosSelLogList.InsertString(0, strFin);
	}
	else if (str == _T("AutoRun 완료"))
	{
		strFin = pAjinLibraryLog->GetStrHowLong(pDIOTestLog->pSequenceDlg->TAutoTime, _T("AutoRun"));
		m_PosLogList.InsertString(0, strFin);
		if (IsPosToday())
			m_PosSelLogList.InsertString(0, strFin);
	}
	
}

void CLogDlg::AddPosLog(int Axis, double Position, CString CStrPos, CString CStrCurTime)
{
	CString strTmp; 
	CString CstrAxis;
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
	
	strTmp.Format(_T("%s | %s위치 | %f %s"), CStrCurTime, CstrAxis, Position, CStrPos);
	m_PosLogList.InsertString(0, strTmp);
	if (IsPosToday())
		m_PosSelLogList.InsertString(0, strTmp);
}

void CLogDlg::AddPosLog(int Axis, CString Time, CString CStrPos, CString CStrCurTime)
{
	CString strTmp;
	CString CstrAxis;
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

	strTmp.Format(_T("%s | %s소요 | %f %s"), CStrCurTime, CstrAxis, Time, CStrPos);
	m_PosLogList.InsertString(0, strTmp);
	if (IsPosToday())
		m_PosSelLogList.InsertString(0, strTmp);
}

void CLogDlg::AddActionLog(CString str, CString CStrCurTime)
{
	CString strTmp;
	strTmp.Format(_T("%s| %s"), CStrCurTime, str);
	this->m_ActionLogList.InsertString(0, strTmp);
	if (IsActionToday())
		m_ActionSelLogList.InsertString(0, strTmp);
}

void CLogDlg::OnDtnDatetimechangePosDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	WORD wSelYear, wSelMonth, wSelDay;
	CStdioFile File;
	BOOL bOpen;

	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	m_PosSelLogList.ResetContent();
	wSelYear = pDTChange->st.wYear;
	wSelMonth = pDTChange->st.wMonth;
	wSelDay = pDTChange->st.wDay;

	setlocale(LC_ALL, "Korean");
	PosSelPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\Position.txt"), wSelYear, wSelMonth, wSelDay);
	bOpen = File.Open(PosSelPath, CStdioFile::modeRead);
	CString strTmp;
	if (bOpen)
	{
		while (File.ReadString(strTmp))
		{
			m_PosSelLogList.InsertString(0,strTmp);
		}
		File.Close();
	}

	FilterPosLog();
}


void CLogDlg::OnDtnDatetimechangeActionDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	WORD wSelYear, wSelMonth, wSelDay;
	CStdioFile File;
	BOOL bOpen;
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	m_ActionSelLogList.ResetContent();
	wSelYear = pDTChange->st.wYear;
	wSelMonth = pDTChange->st.wMonth;
	wSelDay = pDTChange->st.wDay;

	setlocale(LC_ALL, "Korean");
	ActionSelPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\ActionLog.txt"), wSelYear, wSelMonth, wSelDay);
	bOpen = File.Open(ActionSelPath, CStdioFile::modeRead);
	CString strTmp;
	if (bOpen)
	{
		while (File.ReadString(strTmp))
		{
			m_ActionSelLogList.InsertString(0, strTmp);
		}
		File.Close();
	}
	FilterActionLog();
}

void CLogDlg::LoadTodayPosLog()
{
	if (IsPosToday())
	{
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CStdioFile File;
		BOOL bOpen;

		setlocale(LC_ALL, "Korean");
		PosSelPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\Position.txt"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
		bOpen = File.Open(PosSelPath, CStdioFile::modeRead);
		CString strTmp;
		if (bOpen)
		{
			while (File.ReadString(strTmp))
			{
				m_PosSelLogList.InsertString(0, strTmp);
			}
			File.Close();
		}
	}
}

void CLogDlg::LoadTodayActionLog()
{
	if (IsActionToday())
	{
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CStdioFile File;
		BOOL bOpen;

		setlocale(LC_ALL, "Korean");
		ActionSelPath.Format(_T("../Log\\%4d년\\%2d월\\%2d일\\ActionLog.txt"), CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay);
		bOpen = File.Open(ActionSelPath, CStdioFile::modeRead);
		CString strTmp;
		if (bOpen)
		{
			while (File.ReadString(strTmp))
			{
				m_ActionSelLogList.InsertString(0, strTmp);
			}
			File.Close();
		}
	}
}

BOOL CLogDlg::IsPosToday()
{
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	SYSTEMTIME sel_date;
	m_dateTimePos.GetTime(&sel_date);

	if (sel_date.wYear == CurrentTime.wYear && sel_date.wMonth == CurrentTime.wMonth && sel_date.wDay == CurrentTime.wDay)
		return TRUE;
	else
	{
		return FALSE;
	}
}

BOOL CLogDlg::IsActionToday()
{
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	SYSTEMTIME sel_date;
	m_dateTimeAction.GetTime(&sel_date);

	if (sel_date.wYear == CurrentTime.wYear && sel_date.wMonth == CurrentTime.wMonth && sel_date.wDay == CurrentTime.wDay)
		return TRUE;
	else
	{
		return FALSE;
	}
}

CString inline BoolToStr(BOOL b)
{
	CString str;
	b == TRUE ? str = "ON" : str = "OFF";
	return str;
}

void CLogDlg::AddIOLog(int ModuleNo, int nPort, BOOL b)
{
	CString strTmp;

	strTmp.Format(_T("IO Dlg - %d번 Module %d번 Output %s!"), ModuleNo, nPort, BoolToStr(b));
	pAjinLibraryLog->SaveActionLogFile(strTmp);
}

void CLogDlg::OnCbnSelchangePosCombo()
{
	PosFilter = m_PosCombo.GetCurSel();	
	FilterPosLog();
}

void CLogDlg::FilterPosLog()
{
	CStdioFile File;
	BOOL bOpen = File.Open(PosSelPath, CStdioFile::modeRead);
	CString strTmp, str, Substr;
	int startIndex = 12;
	vector<CString> VStrNewPos, VStrPos;
	vector<int> VnSInit, VnEInit, VnSAuto, VnEAuto;
	if (bOpen)
	{
		while (File.ReadString(strTmp))
		{
			VStrPos.push_back(strTmp);
		}
		File.Close();

		switch (PosFilter)
		{
		case X축:
			str = _T(" X축 위치 ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				AfxExtractSubString(Substr, VStrPos[nIndex], 1, '|');
				if (Substr.Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}

			str.Format(_T("X축 이동 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case Y축:
			str = _T(" Y축 위치 ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				AfxExtractSubString(Substr, VStrPos[nIndex], 1, '|');
				if (Substr.Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}

			str.Format(_T("Y축 이동 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case Z축:
			str = _T(" Z축 위치 ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				AfxExtractSubString(Substr, VStrPos[nIndex], 1, '|');
				if (Substr.Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}

			str.Format(_T("Z축 이동 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case READY위치:
			str = _T("(Ready Position) ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}

			str.Format(_T("Ready 위치로 이동한 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case WORKREADY위치:
			str = _T("(Work Ready Position) ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}
			str.Format(_T("Work Ready 위치로 이동한 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case WORKING1위치:
			str = _T("(Working #1 Position) ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}
			str.Format(_T("Working #1 위치로 이동한 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case WORKING2위치:
			str = _T("(Working #2 Position) ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}
			str.Format(_T("Working #2 위치로 이동한 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case WORKING3위치:
			str = _T("(Working #3 Position) ");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}
			str.Format(_T("Working #3 위치로 이동한 횟수: %d"), VStrNewPos.size());
			VStrNewPos.push_back(str);
			break;
		case INITIALIZESEQUENCE:
			str = _T(" Initialize 시작");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VnSInit.push_back(nIndex);
				} 
			}
			str = _T("Initialize 소요시간");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Left(str.GetLength()) == str)
				{
					VnEInit.push_back(nIndex);
				}
			}

			for (int i = 0; i < VnEInit.size(); i++)
			{
				for (int nIndex = VnSInit[i]; nIndex <= VnEInit[i]; nIndex++)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}
			str.Format(_T("Initialize 시행 횟수: %d"), VnEInit.size());
			VStrNewPos.push_back(str);
			break;
		case AUTORUNSEQUENCE:
			str = _T(" AutoRun 시작");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Right(str.GetLength()) == str)
				{
					VnSAuto.push_back(nIndex);
				}
			}
			str = _T("AutoRun 소요시간");
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				if (VStrPos[nIndex].Left(str.GetLength()) == str)
				{
					VnEAuto.push_back(nIndex);
				}
			}

			for (int i = 0; i < VnEAuto.size(); i++)
			{
				for (int nIndex = VnSAuto[i]; nIndex <= VnEAuto[i]; nIndex++)
				{
					VStrNewPos.push_back(VStrPos[nIndex]);
				}
			}
			str.Format(_T("AutoRun 시행 횟수: %d"), VnEAuto.size());
			VStrNewPos.push_back(str);
			break;
		case ALL:
		default:
			for (int nIndex = 0; nIndex < VStrPos.size(); nIndex++)
			{
				VStrNewPos.push_back(VStrPos[nIndex]);
			}
			break;
		}

		m_PosSelLogList.ResetContent();
		for (int nIndex = 0; nIndex < VStrNewPos.size(); nIndex++)
			m_PosSelLogList.InsertString(0, VStrNewPos[nIndex]);
	}
}

void CLogDlg::OnCbnSelchangeActionCombo()
{
	ActionFilter = m_ActionCombo.GetCurSel();
	FilterActionLog();
}

void CLogDlg::FilterActionLog()
{
	CStdioFile File;
	BOOL bOpen = File.Open(ActionSelPath, CStdioFile::modeRead);
	CString strTmp, str;
	int StartIndex = 12;
	CString Dlg;
	vector<CString> VStrNewAct, VStrAct;
	if (bOpen)
	{
		VStrAct.clear();
		VStrNewAct.clear();
		while (File.ReadString(strTmp))
		{
			VStrAct.push_back(strTmp);
		}
		File.Close();

		switch (ActionFilter)
		{
		case 프로그램시작:
			str = _T("Main Dlg - 프로그램시작 이벤트 발생");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				if (VStrAct[nIndex].Mid(StartIndex, VStrAct[nIndex].GetLength()) == str)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			str.Format(_T("프로그램 시작 이벤트 발생 횟수: %d"), VStrNewAct.size());
			VStrNewAct.push_back(str);
			break;
		case 프로그램종료:
			str = _T("Main Dlg - 프로그램종료 이벤트 발생");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				if (VStrAct[nIndex].Mid(StartIndex, VStrAct[nIndex].GetLength()) == str)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			str.Format(_T("프로그램 종료 이벤트 발생 횟수: %d"), VStrNewAct.size());
			VStrNewAct.push_back(str);
			break;

		case OUTPUTON:
			str = _T("Output ON!");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				if (VStrAct[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			str.Format(_T("OUTPUT ON 시도 횟수: %d"), VStrNewAct.size());
			VStrNewAct.push_back(str);
			break;
		case OUTPUTOFF:
			str = _T("Output OFF!");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				if (VStrAct[nIndex].Right(str.GetLength()) == str)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			str.Format(_T("OUTPUT OFF 시도 횟수: %d"), VStrNewAct.size());
			VStrNewAct.push_back(str);
			break;
		case IODIALOG:
			Dlg = _T("IO Dlg ");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				AfxExtractSubString(str, VStrAct[nIndex], NULL, '-');
				if (str.Right(Dlg.GetLength()) == Dlg)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			break;
		case SEQUENCEDIALOG:
			Dlg = _T("Sequence Dlg ");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				AfxExtractSubString(str, VStrAct[nIndex], NULL, '-');
				if (str.Right(Dlg.GetLength()) == Dlg)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			break;
		case SETTINGDIALOG:
			Dlg = _T("Setting Dlg ");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				AfxExtractSubString(str, VStrAct[nIndex], NULL, '-');
				if (str.Right(Dlg.GetLength()) == Dlg)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			break;
		case MAINDIALOG:
			Dlg = _T("Main Dlg ");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				AfxExtractSubString(str, VStrAct[nIndex], NULL, '-');
				if (str.Right(Dlg.GetLength()) == Dlg)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			break;
		case MOTIONDIALOG:
			Dlg = _T("Motion Dlg ");
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				AfxExtractSubString(str, VStrAct[nIndex], NULL, '-');
				if (str.Right(Dlg.GetLength()) == Dlg)
				{
					VStrNewAct.push_back(VStrAct[nIndex]);
				}
			}
			break;
		case ALL:
		default:
			for (int nIndex = 0; nIndex < VStrAct.size(); nIndex++)
			{
				VStrNewAct.push_back(VStrAct[nIndex]);
			}
			break;
		}
		m_ActionSelLogList.ResetContent();
		for (int nIndex = 0; nIndex < VStrNewAct.size(); nIndex++)
			m_ActionSelLogList.InsertString(0, VStrNewAct[nIndex]);
	}
}