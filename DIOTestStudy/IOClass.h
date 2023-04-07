#pragma once


// CIOClass

class CIOClass : public CWnd
{
	DECLARE_DYNAMIC(CIOClass)

public:
	CIOClass();
	virtual ~CIOClass();
	void SetOutput(int ModuleNo, int nPort, BOOL b);
	BOOL GetInput(int ModuleNo, int nPort); 
	BOOL GetOutput(int ModuleNo, int nPort);
	long GetInputCount(int ModuleNo);
	long GetOutputCount(int ModuleNo);
	BOOL ISDioModuleExist();
protected:
	DECLARE_MESSAGE_MAP()
};


