#pragma once



// CutThreadApp
#include "CDialg_Main.h"

class CutThreadApp : public CWinThread
{
	DECLARE_DYNCREATE(CutThreadApp)

protected:
	CutThreadApp();           // 动态创建所使用的受保护的构造函数
	virtual ~CutThreadApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CCDialg_Main* pDlg;
};


