// IO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "IO.h"
#include "AJinLibrary.h"
#include "afxdialogex.h"
#include "DIOTestStudyDlg.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;
// IO 대화 상자입니다.

CIOClass* pIOClass;
CAJinLibrary* pAJinLibraryIO;
CDIOTestStudyDlg *pDIOTestIO;

IMPLEMENT_DYNAMIC(IO, CDialogEx)

IO::IO(CWnd* pParent /*=NULL*/)
	: CDialogEx(IO::IDD, pParent)
{

}


IO::~IO()
{
}

BOOL IO::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pIOClass = new CIOClass;
	pAJinLibraryIO = new CAJinLibrary;
	pDIOTestIO = (CDIOTestStudyDlg *)AfxGetMainWnd();
	IInputCounts = pIOClass->GetInputCount(ModuleNo);
	IOutputCounts = pIOClass->GetOutputCount(ModuleNo);

	//배열에 컨트롤 등록
	for (int i = 0; i < CONTACT_MAX_COUNT; i++)
	{
		m_pchkInput[i] = (CButton *)GetDlgItem(IDC_X0100 + i);
		m_pchkOutput[i] = (CButton *)GetDlgItem(IDC_Y0100 + i);

		m_pchkInputOffsetText[i] = (CButton *)GetDlgItem(IDC_STATIC_X0100 + i);
		m_pchkInputNameText[i] = (CButton *)GetDlgItem(IDC_STATIC_X101 + i);

		m_pchkOutputOffsetText[i] = (CButton *)GetDlgItem(IDC_STATIC_Y0100 + i);
		m_pchkOutputNameText[i] = (CButton *)GetDlgItem(IDC_STATIC_X117 + i);
	}
	
	InitDIOText();      //file 읽어서 문자열 초기화

	InitOffsetButton(); //입력접점, 출력접점을 넘어가는 UI 숨기기

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.

}

void IO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(IO, CDialogEx)
	ON_COMMAND_RANGE(IDC_Y0100, IDC_Y0215, OnClickOutput)
ON_BN_CLICKED(IDC_RADIO1, &IO::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &IO::OnBnClickedRadio2)
END_MESSAGE_MAP()

#pragma region output checkbox click 이벤트
void IO::OnClickOutput(UINT nID)
{
	CString strTmp;
	int nIndex = nID - IDC_Y0100;
	long IModuleCounts = pAJinLibraryIO->GetModuleCount();
	if (IModuleCounts > 0)
	{
		CString strMessage;
		short	nIndex;
		if (pAJinLibraryIO->GetModuleInfo(ModuleNo))
		{
			nIndex = nID - IDC_Y0100;
			if (!m_pchkOutput[nIndex]->GetCheck())
			{
				pIOClass->SetOutput(ModuleNo, nIndex, TRUE); 
				pDIOTestIO->pLogDlg->AddIOLog(ModuleNo, nIndex, TRUE);
			}
			else
			{
				pIOClass->SetOutput(ModuleNo, nIndex, FALSE);
				pDIOTestIO->pLogDlg->AddIOLog(ModuleNo, nIndex, FALSE);
			}
		}

		if (pAJinLibraryIO->GetModuleInfo(ModuleNo))
		{
			nIndex = nID - IDC_Y0100;
			if (m_pchkOutput[nIndex]->GetCheck())
			{
				pDIOTestIO->pLogDlg->AddIOLog(ModuleNo, nIndex, TRUE);
				pIOClass->SetOutput(ModuleNo, nIndex, TRUE);
			}
			else
			{
				pDIOTestIO->pLogDlg->AddIOLog(ModuleNo, nIndex, FALSE);
				pIOClass->SetOutput(ModuleNo, nIndex, FALSE);
			}
		}
		
	}
	
}
#pragma endregion

#pragma region file에서 접점 offset 번호 및 name 읽어서 전시
void IO::InitDIOText()
{
	int nIndex;
	CString path1;
	CString path2;

	CStdioFile file1;
	CStdioFile file2;
	path1 = "../Config\\DioInfo1.txt";
	path2 = "../Config\\DioInfo2.txt";
	vector<CString> vstr1;
	vector<CString> vstr2;
	file1.Open(path1, CStdioFile::modeRead);  //file을 열고 읽음
	file2.Open(path2, CStdioFile::modeRead);  //file을 열고 읽음
	
	CString strTmp;

	while (file1.ReadString(strTmp))
	{
		vstr1.push_back(strTmp);
	}
	while (file2.ReadString(strTmp))
	{
		vstr2.push_back(strTmp);
	}
	file1.Close();
	file2.Close();
	if (ModuleNo == 0)
	{
		for (nIndex = 0; nIndex < IInputCounts; nIndex++)
		{
			wstring str = CT2CW(vstr1[nIndex]);
			wstring offsetstr = str.substr(6, 6);
			m_pchkInputOffsetText[nIndex]->SetWindowText(offsetstr.c_str());

			str = str.substr(13, str.length());
			m_pchkInputNameText[nIndex]->SetWindowText(str.c_str());
		}
		for (nIndex = 0; nIndex < IOutputCounts; nIndex++)
		{
			wstring str2 = CT2CW(vstr1[nIndex + IInputCounts]);
			wstring offsetstr2 = str2.substr(7, 6);
			m_pchkOutputOffsetText[nIndex]->SetWindowText(offsetstr2.c_str());

			str2 = str2.substr(14, str2.length());
			m_pchkOutputNameText[nIndex]->SetWindowText(str2.c_str());
		}
	}
	else if (ModuleNo == 1)
	{
		for (nIndex = 0; nIndex < IInputCounts; nIndex++)
		{
			wstring str = CT2CW(vstr2[nIndex]);
			wstring offsetstr = str.substr(6, 6);
			m_pchkInputOffsetText[nIndex]->SetWindowText(offsetstr.c_str());
			str = str.substr(13, str.length());
			m_pchkInputNameText[nIndex]->SetWindowText(str.c_str());
		}
		for (nIndex = 0; nIndex < IOutputCounts; nIndex++)
		{
			wstring str2 = CT2CW(vstr2[nIndex + IInputCounts]);
			wstring offsetstr2 = str2.substr(7, 6);
			m_pchkOutputOffsetText[nIndex]->SetWindowText(offsetstr2.c_str());

			str2 = str2.substr(14, str2.length());
			m_pchkOutputNameText[nIndex]->SetWindowText(str2.c_str());
		}
	}

	else
	{
		for (nIndex = 0; nIndex < CONTACT_MAX_COUNT; nIndex++)
		{
			m_pchkInputOffsetText[nIndex]->SetWindowText(NULL);
			m_pchkOutputOffsetText[nIndex]->SetWindowText(NULL);
		}
	}

}
#pragma endregion

#pragma region 입력접점, 출력접점을 넘어가는 UI 숨기기
void IO::InitOffsetButton()
{
	//for (int i = IInputCounts; i < CONTACT_MAX_COUNT; i++)
	//{
	//	m_pchkInput[i]->ShowWindow(0);// disable it
	//	m_pchkInputOffsetText[i]->ShowWindow(0);
	//}
	//for (int i = IOutputCounts; i < CONTACT_MAX_COUNT; i++)
	//{
	//	m_pchkOutput[i]->ShowWindow(0);// disable it
	//	m_pchkOutputOffsetText[i]->ShowWindow(0);
	//}
}
#pragma endregion

#pragma region 보고싶은 모듈 선택, 선택된 모듈별로 파일에서 문자열 읽어서 보여주기
void IO::OnBnClickedRadio1()
{
	pAJinLibraryIO->SaveActionLogFile(_T("IO Dlg - 00번 Module 선택 이벤트 발생!"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ModuleNo = 0;
	IInputCounts = pIOClass->GetInputCount(ModuleNo);
	IOutputCounts = pIOClass->GetOutputCount(ModuleNo);
	InitDIOText();
	InitOffsetButton();
}


void IO::OnBnClickedRadio2()
{
	pAJinLibraryIO->SaveActionLogFile(_T("IO Dlg - 01번 Module 선택 이벤트 발생!"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ModuleNo = 1;
	IInputCounts = pIOClass->GetInputCount(ModuleNo);
	IOutputCounts = pIOClass->GetOutputCount(ModuleNo);
	InitDIOText();
	InitOffsetButton();
}

#pragma endregion

