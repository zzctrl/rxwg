#pragma once


// CCDialg_Main 对话框
#include "Resource.h"
#include "Funtion.h"
#include "afxwin.h"
#include "PlayHelper.h"
#include "ListBoxEx.h"
#include <map>

// 挂机相关消息，由游戏线程处理
// 通知游戏线程进行初始化
#define RXJHMSG_INITIALIZE			WM_USER + 0x600
// 更新设置数据
#define RXJHMSG_UPDATE_SETTING		WM_USER + 0x601  
// 开始/停止挂机,wparam:1为开始挂机，0停止挂机
#define RXJHMSG_AUTOWORK			WM_USER + 0x602  
// 获取当前坐标
#define RXJHMSG_GETPOINT			WM_USER + 0x603


// 设置窗口处理的消息
// 从本地文件加载配置数据
#define RXJHMSG_LOADCONFIG			WM_USER + 0x700
// 刷新界面
#define RXJHMSG_REFRESHUI			WM_USER + 0x701


// 定时器ID
// 保护(自动加血，蓝)
#define TIMERID_PROTECT			0x1000   
// 挂机打怪/回城补给等
#define TIMERID_WORK			0x1001   
// 加辅助
#define TIMERID_FUZHU			0x1002


class CCDialg_Main : public CDialog
{
	DECLARE_DYNAMIC(CCDialg_Main)

public:
	CCDialg_Main(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCDialg_Main();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

	void SetGameWnd(HWND a_hwnd);
	// 设置角色当前坐标
	void SetPoint(POINT a_pt);

	// 获取设置数据，在游戏线程会访问，需要加锁
	Config GetConfigData();
	// 加载角色的配置
	void LoadConfigByName(const CString& a_roleName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnMsgLoadConfig(WPARAM a_wparam, LPARAM a_lparam);
	afx_msg LRESULT OnMsgRefreshUI(WPARAM a_wparam, LPARAM a_lparam);
private:
	void ReadData();

	void AttackWork();
	void ProtectWork();

	void NotifyMessage(UINT a_msg, WPARAM a_wparam = 0, LPARAM a_lparam = 0);

	int m_nHong;
	int m_nLan;
	int m_nX;
	int m_nY;

	bool m_bWorking;

	PlayHelper m_playHelper;
public:
	CButton m_btnWork;
	//CString m_szMapName;
	DWORD m_nAttackRange;
	DWORD m_nProtectHP;
	DWORD m_nProtectMP;
	BOOL m_bNearest;
	CComboBox m_attackType;
	CComboBox m_HPList;
	CComboBox m_MPList;
	// 游戏窗口
	HWND m_hGameWnd;
	Config m_cfg;
	// 临界区，保护设置数据
	CCriticalSection m_cs;
	
	afx_msg void OnGetCurrentPoint();
	afx_msg void OnApplyConfig();
	afx_msg void OnCbnDropdownHPList();
	CComboBox m_comboMap;
	afx_msg void OnLoadConfig();
	CComboBox m_comboSupply;
	BOOL m_bPackageFull;
	BOOL m_bArrows;
	BOOL m_bMPCounts;
	BOOL m_bHPCounts;
	int m_nHPCounts;
	int m_nMPCounts;
	int m_nArrowCounts;
	CComboBox m_comboBuyList;
	DWORD m_nBuyCount;
	afx_msg void OnAddBuyGoods();
	CListBoxEx m_listBuys;
	std::map<CString, DWORD> m_buys;
	afx_msg void OnModifyBuyItem();
	afx_msg void OnDeleteBuyItem();
	afx_msg void OnDblclkListBuy();
};
