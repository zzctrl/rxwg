// RXJH_GameDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RXJH_GameDll.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRXJH_GameDllApp

BEGIN_MESSAGE_MAP(CRXJH_GameDllApp, CWinApp)
END_MESSAGE_MAP()


// CRXJH_GameDllApp 构造

CRXJH_GameDllApp::CRXJH_GameDllApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pThread = NULL;
	m_hGameWnd = NULL;
	m_pGameWndProc = NULL;
	m_bInitialize = false;
}


// 唯一的一个 CRXJH_GameDllApp 对象

CRXJH_GameDllApp theApp;


// CRXJH_GameDllApp 初始化

BOOL CRXJH_GameDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

LRESULT CRXJH_GameDllApp::ProcessMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT ret = 1;
	// 先让游戏处理消息
	if (m_pGameWndProc)
	{
		ret = m_pGameWndProc(hwnd, msg, wparam, lparam);
	}

	// 首先判断游戏数据是否还有效，比如切换人物或退出
	if (!m_playHelper.IsDataValid())
	{
		if (m_bInitialize)
		{
			StopWork();
		}
		return ret;
	}
	// 处理自定义消息
	switch (msg)
	{
	case RXJHMSG_INITIALIZE:
	{
		OnMsgInitialize();
	}
	break;
	case RXJHMSG_UPDATE_SETTING:
	{
		OnMsgUpdateSetting(wparam != 0);
	}
	break;
	case RXJHMSG_AUTOWORK:
	{
		bool bWork = wparam != 0;
		OnMsgAutoWork(bWork);
	}
	break;
	case RXJHMSG_GETPOINT:
	{
		OnMsgGetPoint();
	}
	break;
	case WM_TIMER:
	{
		OnTimer(wparam);
	}
	break;
	default:
		break;
	}
	return ret;
}

LRESULT CALLBACK myWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return theApp.ProcessMessage(hwnd, msg, wparam, lparam);
}

// 钩子句柄
HHOOK g_hHook = NULL;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (lParam & 0x80000000)
	{
		if (wParam == VK_F12)
		{
			theApp.AttachToGame();
		}
	}
	return CallNextHookEx(g_hHook, code, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char szClass[128] = { 0 };
	::GetClassNameA(hwnd, szClass, 128);
	if (strcmp(szClass, "D3D Window") == 0)
	{
		DWORD dwThreadID;
		dwThreadID = ::GetWindowThreadProcessId(hwnd, NULL);
		g_hHook = ::SetWindowsHookExA(WH_KEYBOARD, KeyboardProc, ::GetModuleHandle("RXJH_GameDll.dll"), dwThreadID);
	}
	return TRUE;
}

HHOOK g_wndProcHook = NULL;

LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HCBT_CREATEWND == nCode)
	{
		HWND hwnd = (HWND)wParam;
		if (hwnd)
		{
			char szClass[128] = { 0 };
			::GetClassNameA(hwnd, szClass, 128);
			CString szmsg = "rxjh: ";
			szmsg += szClass;
			::OutputDebugStringA(szmsg);
			if (strcmp(szClass, "D3D Window") == 0)
			{
				DWORD dwThreadID;
				dwThreadID = ::GetWindowThreadProcessId(hwnd, NULL);
				g_hHook = ::SetWindowsHookExA(WH_KEYBOARD, KeyboardProc, ::GetModuleHandle("RXJH_GameDll.dll"), dwThreadID);
			}
		}
	}
	return ::CallNextHookEx(g_wndProcHook, nCode, wParam, lParam);
}

void SetDllHookClient()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// test
	::EnumWindows(EnumWindowsProc, NULL);

	// 设置创建窗口消息的钩子
	//g_wndProcHook = ::SetWindowsHookExA(WH_CBT, MessageProc, ::GetModuleHandle("RXJH_GameDll.dll"), 0);
	// endtest

	// D3D Window
	//HWND hwnd = ::FindWindowA(NULL,"YB_OnlineClient");
	/*HWND hwnd = ::FindWindowA("D3D Window", NULL);
	if (NULL == hwnd)
	{
		AfxMessageBox("游戏未打开，请重试！");
		return;
	}
	DWORD dwThreadID;
	dwThreadID = ::GetWindowThreadProcessId(hwnd,NULL);
	g_hHook = ::SetWindowsHookExA(WH_KEYBOARD,KeyboardProc,::GetModuleHandle("RXJH_GameDll.dll"),dwThreadID);*/
}

int CRXJH_GameDllApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	// 恢复游戏窗口的过程函数
	if (m_pGameWndProc)
	{
		::SetWindowLongA(m_hGameWnd, GWL_WNDPROC, (LONG)m_pGameWndProc);
	}
	// 取消钩子
	if (g_hHook)
	{
		::UnhookWindowsHookEx(g_hHook);
	}
	if (g_wndProcHook)
	{
		::UnhookWindowsHookEx(g_wndProcHook);
	}
	
	return CWinApp::ExitInstance();
}

// 挂载到游戏
void CRXJH_GameDllApp::AttachToGame()
{
	if (NULL == m_pThread)
	{
		if (NULL == m_hGameWnd)
		{
			m_hGameWnd = ::GetForegroundWindow();
		}
		m_pThread = (CutThreadApp*)AfxBeginThread(RUNTIME_CLASS(CutThreadApp));
		m_pThread->SetGameWnd(m_hGameWnd);
		if (NULL == m_pGameWndProc)
		{
			m_pGameWndProc = (WNDPROC)::SetWindowLongA(m_hGameWnd, GWL_WNDPROC, (LONG)myWndProc);
		}
	}
	else
	{
		m_pThread->ShowSettingWnd();
	}
}

void CRXJH_GameDllApp::OnMsgInitialize()
{
	m_bInitialize = true;

	// 初始化当前角色属性等信息
	m_playHelper.Initialize();

	// 通知设置窗口加载配置
	CCDialg_Main* pDlg = m_pThread->GetSettingDlg();
	char* pRoleName = m_playHelper.GetRoleName();
	pDlg->PostMessageA(RXJHMSG_LOADCONFIG, (WPARAM)pRoleName);
	// 设置当前游戏窗口标题
	::SetWindowTextA(m_hGameWnd, pRoleName);
}

// 更新设置
void CRXJH_GameDllApp::OnMsgUpdateSetting(bool a_bProtect)
{
	CCDialg_Main* pDlg = m_pThread->GetSettingDlg();
	if (pDlg)
	{
		m_playHelper.UpdateConfig(pDlg->GetConfigData());
	}
	// 开始保护处理
	if (a_bProtect)
	{
		::SetTimer(m_hGameWnd, TIMERID_PROTECT, 200, NULL);
	}
}
// 开始/停止挂机
void CRXJH_GameDllApp::OnMsgAutoWork(bool a_bWork)
{
	if (a_bWork)
	{
		m_playHelper.Start();
		::SetTimer(m_hGameWnd, TIMERID_WORK, 400, NULL);
	}
	else
	{
		m_playHelper.Pause();
		::KillTimer(m_hGameWnd, TIMERID_WORK);
	}
}
void CRXJH_GameDllApp::OnMsgGetPoint()
{
	POINT pt = m_playHelper.GetCurPoint();
	m_playHelper.SetWorkPoint(pt);

	CCDialg_Main* pDlg = m_pThread->GetSettingDlg();
	if (pDlg)
	{
		pDlg->SetPoint(pt);
	}
}
// 定时器消息处理
void CRXJH_GameDllApp::OnTimer(DWORD a_timerID)
{
	switch (a_timerID)
	{
	case TIMERID_PROTECT:
		m_playHelper.Protect();
		break;
	case TIMERID_WORK:
		m_playHelper.Work();
		break;
	default:
		break;
	}
}

void CRXJH_GameDllApp::StopWork()
{
	m_bInitialize = false;

	::KillTimer(m_hGameWnd, TIMERID_PROTECT);
	::KillTimer(m_hGameWnd, TIMERID_WORK);
	// 销毁设置窗口
	m_pThread->PostThreadMessageA(RXJHMSG_DESTROY_SETTINGDLG, 0, 0);
	m_pThread = NULL;
	// 重新设置游戏窗口标题
	::SetWindowTextA(m_hGameWnd, "YB_OnlineClient");
	// 停止工作并清空设置数据
	m_playHelper.Reset();
}
