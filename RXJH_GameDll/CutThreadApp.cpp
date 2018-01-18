// CutThreadApp.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "CutThreadApp.h"


// CutThreadApp

IMPLEMENT_DYNCREATE(CutThreadApp, CWinThread)

CutThreadApp::CutThreadApp()
{
	pDlg = nullptr;
}

CutThreadApp::~CutThreadApp()
{
}

BOOL CutThreadApp::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	if (nullptr == pDlg)
	{
		pDlg = new CCDialg_Main;
		pDlg->Create(IDD_DIALOG_MAIN);
		pDlg->ShowWindow(SW_SHOW);
		pDlg->UpdateData();
	}
	return TRUE;
}

int CutThreadApp::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	delete pDlg;
	pDlg = nullptr;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CutThreadApp, CWinThread)
END_MESSAGE_MAP()


// CutThreadApp 消息处理程序
