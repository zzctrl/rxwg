// CutThreadApp.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "CutThreadApp.h"


// CutThreadApp

IMPLEMENT_DYNCREATE(CutThreadApp, CWinThread)

CutThreadApp::CutThreadApp()
{
	m_pDlg = NULL;
	m_hGameWnd = NULL;
}

CutThreadApp::~CutThreadApp()
{
}

void CutThreadApp::SetGameWnd(HWND a_hwnd)
{
	m_hGameWnd = a_hwnd;
}
HWND CutThreadApp::GetSettingWnd()
{
	HWND hwnd = NULL;
	if (m_pDlg)
	{
		hwnd = m_pDlg->GetSafeHwnd();
	}
	return hwnd;
}

CCDialg_Main* CutThreadApp::GetSettingDlg()
{
	return m_pDlg;
}

BOOL CutThreadApp::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CreateSettingWnd();

	return TRUE;
}

int CutThreadApp::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	
	return CWinThread::ExitInstance();
}

void CutThreadApp::ShowSettingWnd()
{
	if (m_pDlg)
	{
		m_pDlg->ShowWindow(SW_SHOW);
	}
}

void CutThreadApp::OnMsgCreateSettingDlg(WPARAM a_wpram, LPARAM a_lpram)
{
	CreateSettingWnd();
}

void CutThreadApp::OnMsgDestroySettingDlg(WPARAM a_wpram, LPARAM a_lpram)
{
	DestroySettingWnd();
}

void CutThreadApp::CreateSettingWnd()
{
	if (NULL == m_pDlg)
	{
		m_pDlg = new CCDialg_Main;
		m_pDlg->SetGameWnd(m_hGameWnd);
		m_pDlg->Create(IDD_DIALOG_MAIN);
		m_pDlg->ShowWindow(SW_SHOW);
		m_pDlg->UpdateData();
	}
}

void CutThreadApp::DestroySettingWnd()
{
	if (m_pDlg)
	{
		m_pDlg->SendMessageA(WM_SYSCOMMAND, SC_CLOSE, 0);

		delete m_pDlg;
		m_pDlg = NULL;
	}
}

BEGIN_MESSAGE_MAP(CutThreadApp, CWinThread)
	ON_THREAD_MESSAGE(RXJHMSG_CREATE_SETTINGDLG, &CutThreadApp::OnMsgCreateSettingDlg)
	ON_THREAD_MESSAGE(RXJHMSG_DESTROY_SETTINGDLG, &CutThreadApp::OnMsgDestroySettingDlg)
END_MESSAGE_MAP()


// CutThreadApp 消息处理程序
