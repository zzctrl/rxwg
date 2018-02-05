#pragma once



// CutThreadApp
#include "CDialg_Main.h"


#define RXJHMSG_CREATE_SETTINGDLG		WM_USER + 0x1000
#define RXJHMSG_DESTROY_SETTINGDLG		WM_USER + 0x1001


class CutThreadApp : public CWinThread
{
	DECLARE_DYNCREATE(CutThreadApp)

protected:
	CutThreadApp();           // 动态创建所使用的受保护的构造函数
	virtual ~CutThreadApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CCDialg_Main* GetSettingDlg();

	void SetGameWnd(HWND a_hwnd);
	HWND GetSettingWnd();
	void ShowSettingWnd();

private:
	void CreateSettingWnd();
	void DestroySettingWnd();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMsgCreateSettingDlg(WPARAM a_wpram, LPARAM a_lpram);
	afx_msg void OnMsgDestroySettingDlg(WPARAM a_wpram, LPARAM a_lpram);
public:
	CCDialg_Main* m_pDlg;
	HWND m_hGameWnd;
};


