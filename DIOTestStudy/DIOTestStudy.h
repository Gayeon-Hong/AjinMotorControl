
// DIOTestStudy.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDIOTestStudyApp:
// �� Ŭ������ ������ ���ؼ��� DIOTestStudy.cpp�� �����Ͻʽÿ�.
//

class CDIOTestStudyApp : public CWinApp
{
public:
	CDIOTestStudyApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDIOTestStudyApp theApp;