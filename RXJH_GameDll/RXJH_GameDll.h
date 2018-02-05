// RXJH_GameDll.h : RXJH_GameDll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "CutThreadApp.h"
#include "Config.h"
#include "PlayHelper.h"

// CRXJH_GameDllApp
// 有关此类实现的信息，请参阅 RXJH_GameDll.cpp
//

class CRXJH_GameDllApp : public CWinApp
{
public:
	CRXJH_GameDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();


public:

	// 挂载到游戏
	void AttachToGame();

	// 消息处理
	LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	// 初始化
	void OnMsgInitialize();
	// 更新设置
	void OnMsgUpdateSetting(bool a_bProtect);
	// 开始/停止挂机
	void OnMsgAutoWork(bool a_bWork);
	// 获取角色当前坐标
	void OnMsgGetPoint();

	// 定时器消息处理
	void OnTimer(DWORD a_timerID);

	// 停止工作
	void StopWork();

private:

	// 游戏辅助类
	PlayHelper m_playHelper;

	// 设置窗口线程
	CutThreadApp* m_pThread;
	// 游戏窗口句柄
	HWND m_hGameWnd;
	// 窗口过程函数地址
	WNDPROC m_pGameWndProc;

	// 
	bool m_bInitialize;
};
