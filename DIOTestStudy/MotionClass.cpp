// MotionClass.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIOTestStudy.h"
#include "MotionClass.h"


// CMotionClass

IMPLEMENT_DYNAMIC(CMotionClass, CWnd)

CMotionClass::CMotionClass()
{

}

CMotionClass::~CMotionClass()
{

}


BEGIN_MESSAGE_MAP(CMotionClass, CWnd)
END_MESSAGE_MAP()


long CMotionClass::GetAxisCount()
{
	long m_lAxisCount;

	AxmInfoGetAxisCount(&m_lAxisCount);

	return m_lAxisCount;
}

void CMotionClass::SetHomeResult(long m_lAxisNo)
{
	AxmHomeSetResult(m_lAxisNo, HOME_ERR_UNKNOWN);
}

DWORD CMotionClass::GetHomeResult(long m_lAxisNo)
{
	DWORD dwState;

	AxmHomeGetResult(m_lAxisNo, &dwState);

	return dwState;
}
BOOL CMotionClass::GetHomeBoolResult(long m_lAxisNo)
{
	DWORD dwState;

	AxmHomeGetResult(m_lAxisNo, &dwState);
	if (dwState == HOME_SUCCESS)
		return TRUE;
	else
	{
		return FALSE;
	}
	//return dwState;
}


DWORD CMotionClass::GetHomeMainRate(long m_lAxisNo)
{
	DWORD dwStepMain;
	DWORD dwRetCode = AxmHomeGetRate(m_lAxisNo, &dwStepMain, NULL);
	if (!dwRetCode == AXT_RT_SUCCESS)
	{
		return -1;
	}
	else
	{
		return dwStepMain;
	}
}

DWORD CMotionClass::GetHomeSubRate(long m_lAxisNo)
{
	DWORD dwStepSub;

	DWORD dwRetCode = AxmHomeGetRate(m_lAxisNo, NULL, &dwStepSub);
	if (!dwRetCode == AXT_RT_SUCCESS)
	{
		return -1;
	}
	else
	{
		return dwStepSub;
	}
}

void CMotionClass::ClearAllPosZero(long m_lAxisNo)
{
	AxmStatusSetPosMatch(m_lAxisNo, 0.0);
}

#pragma region homesearch 결과 문자열 출력을 위한 함수
CString  CMotionClass::HomeSearchResultDw2Csting(DWORD dwHomeResult)
{
	CString strResult;

	switch (dwHomeResult)
	{
	case HOME_SUCCESS:			strResult.Format(_T("[01H] HOME_SUCCESS"));			break;
	case HOME_SEARCHING:		strResult.Format(_T("[02H] HOME_SEARCHING"));		break;
	case HOME_ERR_GNT_RANGE:	strResult.Format(_T("[10H] HOME_ERR_GNT_RANGE"));	break;
	case HOME_ERR_USER_BREAK:	strResult.Format(_T("[11H] HOME_ERR_USER_BREAK"));	break;
	case HOME_ERR_VELOCITY:		strResult.Format(_T("[12H] HOME_ERR_VELOCITY"));	break;
	case HOME_ERR_AMP_FAULT:	strResult.Format(_T("[13H] HOME_ERR_AMP_FAULT"));	break;
	case HOME_ERR_NEG_LIMIT:	strResult.Format(_T("[14H] HOME_ERR_NEG_LIMIT"));	break;
	case HOME_ERR_POS_LIMIT:	strResult.Format(_T("[15H] HOME_ERR_POS_LIMIT"));	break;
	case HOME_ERR_NOT_DETECT:	strResult.Format(_T("[16H] HOME_ERR_NOT_DETECT"));	break;
	case HOME_ERR_UNKNOWN:		strResult.Format(_T("[FFH] HOME_ERR_UNKNOWN"));		break;
	}

	return strResult;
}
#pragma endregion

void CMotionClass::MoveJog(long m_lAxisNo, double dJogVel, double dJogAcc, double dJogDec)
{
	CString strData;
	DWORD dwRetCode = AxmMoveVel(m_lAxisNo, dJogVel, dJogAcc, dJogDec);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveVel return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CMotionClass::Estop(long m_lAxisNo)
{
	CString strData;
	DWORD	dwRetCode;

	dwRetCode = AxmMoveEStop(m_lAxisNo);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveEStop return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CMotionClass::Sstop(long m_lAxisNo)
{
	CString strData;
	DWORD	dwRetCode;

	dwRetCode = AxmMoveSStop(m_lAxisNo);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveSStop return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CMotionClass::Stop(long m_lAxisNo, double dDecel)
{
	CString strData;
	DWORD	dwRetCode = AxmMoveStop(m_lAxisNo, dDecel);
	if (dwRetCode != AXT_RT_SUCCESS)
	{
		strData.Format(_T("AxmMoveStop return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CMotionClass::SingleMove(long m_lAxisNo, double dMovePos, double dMoveVel, double dMoveAcc, double dMoveDec)
{
	CString strData;
	DWORD	dwRetCode;

	dwRetCode = AxmMoveStartPos(m_lAxisNo, dMovePos, dMoveVel, dMoveAcc, dMoveDec);
	if (dwRetCode != AXT_RT_SUCCESS){
		strData.Format(_T("AxmMoveStartPos return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}
void CMotionClass::MovePos(long m_lAxisNo, double dMovePos, double dMoveVel, double dMoveAcc, double dMoveDec)
{
	CString strData;
	DWORD	dwRetCode;

	dwRetCode = AxmMovePos(m_lAxisNo, dMovePos, dMoveVel, dMoveAcc, dMoveDec);
	if (dwRetCode != AXT_RT_SUCCESS){
		strData.Format(_T("AxmMoveStartPos return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

BOOL CMotionClass::StartHomeSearch(long m_lAxisNo)
{
	DWORD	dwRetCode;

	//++ 지정한 축에 원점검색을 진행합니다.
	dwRetCode = AxmHomeSetStart(m_lAxisNo);
	if (dwRetCode != AXT_RT_SUCCESS){
		CString strData;
		strData.Format(_T("AxmHomeSetStart return error[Code:%04d]"), dwRetCode);
		AfxMessageBox(strData);
		return FALSE;
	}
	return TRUE;
}

void CMotionClass::SetAbsMode(long m_lAxisNo)
{

	CString strData;
	DWORD	dwRetCode;
	dwRetCode = AxmMotSetAbsRelMode(m_lAxisNo, POS_ABS_MODE);
	if (dwRetCode != AXT_RT_SUCCESS){
		strData.Format(_T("AxmMotSetAbsRelMode return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CMotionClass::SetRelMode(long m_lAxisNo)
{

	CString strData;
	DWORD	dwRetCode;
	dwRetCode = AxmMotSetAbsRelMode(m_lAxisNo, POS_ABS_LONG_MODE);
	if (dwRetCode != AXT_RT_SUCCESS){
		strData.Format(_T("AxmMotSetAbsRelMode return error[Code:%04d]"), dwRetCode);
		MessageBox(strData, _T("Error"), MB_OK | MB_ICONERROR);
	}
}

// CMotionClass 메시지 처리기입니다.


