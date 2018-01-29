// RXJH_GameDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RXJH_GameDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CWinThread* pThread;
//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CRXJH_GameDllApp

BEGIN_MESSAGE_MAP(CRXJH_GameDllApp, CWinApp)
END_MESSAGE_MAP()


// CRXJH_GameDllApp 构造

CRXJH_GameDllApp::CRXJH_GameDllApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	pThread = nullptr;
}


// 唯一的一个 CRXJH_GameDllApp 对象

CRXJH_GameDllApp theApp;


// CRXJH_GameDllApp 初始化

BOOL CRXJH_GameDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

LRESULT CALLBACK KeyboardProc(int code,
							  WPARAM wParam,
							  LPARAM lParam
							  )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (lParam & 0x80000000)
	{
		if (wParam == VK_F12)
		{
			if (nullptr == pThread)
			{
				pThread = AfxBeginThread(RUNTIME_CLASS(CutThreadApp));
			}
		}
	}
	return CallNextHookEx(nullptr,0,wParam,lParam);
}

void SetDllHookClient()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// test
	//pThread = AfxBeginThread(RUNTIME_CLASS(CutThreadApp));
	//return;
	// endtest
	HWND hwnd = ::FindWindow(nullptr,"YB_OnlineClient");
	if (nullptr == hwnd)
	{
		AfxMessageBox("游戏未打开，请重试！");
		return;
	}
	DWORD dwThreadID;
	dwThreadID = ::GetWindowThreadProcessId(hwnd,nullptr);
	::SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,::GetModuleHandle("RXJH_GameDll.dll"),dwThreadID);
}

int CRXJH_GameDllApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CWinApp::ExitInstance();
}
