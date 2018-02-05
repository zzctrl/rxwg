// CDialg_Main.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "CDialg_Main.h"
#include "afxdialogex.h"
#include "CommonRead.h"
#include "AddressData.h"
#include "EntityPlayer.h"
#include "Config.h"
#include "BuyCountDlg.h"
#include "EntityNPC.h"


// CCDialg_Main 对话框
// 自动回补红，蓝
#define TIMERID_PROTECT			0x1000
// 挂机打怪
#define TIMERID_ATTACK			0x1001



IMPLEMENT_DYNAMIC(CCDialg_Main, CDialog)

CCDialg_Main::CCDialg_Main(CWnd* pParent /*=NULL*/)
	: CDialog(CCDialg_Main::IDD, pParent)
	, m_nHong(0)
	, m_nLan(0)
	, m_nX(0)
	, m_nY(0)
	//, m_szMapName(_T(""))
	, m_nAttackRange(150)
	, m_nProtectHP(150)
	, m_nProtectMP(100)
	, m_bNearest(TRUE)
	, m_bPackageFull(FALSE)
	, m_bArrows(FALSE)
	, m_bMPCounts(FALSE)
	, m_bHPCounts(TRUE)
	, m_nHPCounts(3)
	, m_nMPCounts(3)
	, m_nArrowCounts(3)
	, m_nBuyCount(100)
{
	m_bWorking = false;
	m_hGameWnd = NULL;
}

CCDialg_Main::~CCDialg_Main()
{

}

void CCDialg_Main::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_nX);
	DDX_Text(pDX, IDC_EDIT3, m_nY);
	DDX_Control(pDX, IDC_BUTTON, m_btnWork);
	//DDX_Text(pDX, IDC_EDIT8, m_szMapName);
	DDX_Text(pDX, IDC_EDIT5, m_nAttackRange);
	DDX_Text(pDX, IDC_EDIT6, m_nProtectHP);
	DDX_Text(pDX, IDC_EDIT7, m_nProtectMP);
	DDX_Check(pDX, IDC_CHECK1, m_bNearest);
	DDX_Control(pDX, IDC_COMBO1, m_attackType);
	DDX_Control(pDX, IDC_COMBO2, m_HPList);
	DDX_Control(pDX, IDC_COMBO3, m_MPList);
	DDX_Control(pDX, IDC_COMBO4, m_comboMap);
	DDX_Control(pDX, IDC_COMBO5, m_comboSupply);
	DDX_Check(pDX, IDC_CHECK5, m_bPackageFull);
	DDX_Check(pDX, IDC_CHECK4, m_bArrows);
	DDX_Check(pDX, IDC_CHECK3, m_bMPCounts);
	DDX_Check(pDX, IDC_CHECK2, m_bHPCounts);
	DDX_Text(pDX, IDC_EDIT8, m_nHPCounts);
	DDX_Text(pDX, IDC_EDIT9, m_nMPCounts);
	DDX_Text(pDX, IDC_EDIT10, m_nArrowCounts);
	DDX_Control(pDX, IDC_COMBO6, m_comboBuyList);
	DDX_Text(pDX, IDC_EDIT11, m_nBuyCount);
	DDX_Control(pDX, IDC_LIST1, m_listBuys);
}


BEGIN_MESSAGE_MAP(CCDialg_Main, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON, &CCDialg_Main::OnBnClickedButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CCDialg_Main::OnGetCurrentPoint)
	ON_BN_CLICKED(IDC_BUTTON3, &CCDialg_Main::OnApplyConfig)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CCDialg_Main::OnCbnDropdownHPList)
	ON_BN_CLICKED(IDC_BUTTON4, &CCDialg_Main::OnLoadConfig)
	ON_BN_CLICKED(IDC_BUTTON5, &CCDialg_Main::OnAddBuyGoods)
	ON_COMMAND(ID_MODIFY, &CCDialg_Main::OnModifyBuyItem)
	ON_COMMAND(ID_DELETE, &CCDialg_Main::OnDeleteBuyItem)
	ON_LBN_DBLCLK(IDC_LIST1, &CCDialg_Main::OnDblclkListBuy)
	ON_MESSAGE(RXJHMSG_LOADCONFIG, &CCDialg_Main::OnMsgLoadConfig)
	ON_MESSAGE(RXJHMSG_REFRESHUI, &CCDialg_Main::OnMsgRefreshUI)
END_MESSAGE_MAP()


// CCDialg_Main 消息处理程序


void CCDialg_Main::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_DESTROY,0,0);
	CDialog::OnClose();
}


void CCDialg_Main::OnDestroy()
{
	CDialog::OnDestroy();
	PostQuitMessage(0);
	// TODO: 在此处添加消息处理程序代码
}

void buyfun()
{
	BYTE* pDate = new BYTE[142];
	memset(pDate, 0, 142);
	//BYTE data[142] = { 00, 00, 0x92, 00, 0x88, 00, 01, 00, 00, 00, 0x50, 0xE4, 0xB9, 0x28, 0x66, 0xCA, 0x9A, 0x3B, 00, 00, 00, 00, 01};
	_asm
	{
		mov eax, pDate
		mov dword ptr[eax + 2], 0x00880092
		mov dword ptr[eax + 6], 0x1
		mov dword ptr[eax + 0x0A], 0x28B9E450
		mov dword ptr[eax + 0x0E], 0x3B9ACA65
		mov dword ptr[eax + 0x12], 0
		mov dword ptr[eax + 0x16], 0x1
		mov ecx, dword ptr ds : [0x1097760]
		push 0x8E
		push eax
		mov edx, 0x004E0220
		call edx
	}
	delete pDate;
	pDate = nullptr;
	/*_asm
	{
		sub esp, 0x8E
		mov eax, esp
		mov dword ptr[eax + 2], 0x00880092
		mov dword ptr[eax + 6], 0x1
		mov dword ptr[eax + 0x0A], 0x28B9E450
		mov dword ptr[eax + 0x0E], 0x3B9ACA65
		mov dword ptr[eax + 0x12], 0
		mov dword ptr[eax + 0x16], 0x1
		mov ecx, dword ptr ds : [0x1097760]
		push 0x8E
		push eax
		mov edx, 0x004E0220
		call edx
		add esp, 0x8E
	}*/
}

void CCDialg_Main::OnBnClickedButton()
{
	//buyfun();
	//CheckNPC(0x15a3);
	//OpenNPCTalk(0x15a3);
	//return;
	// TODO: 在此添加控件通知处理程序代码

	if (!m_bWorking)
	{
		m_btnWork.SetWindowTextA("停止挂机");
	}
	else
	{
		m_btnWork.SetWindowTextA("开始挂机");
	}
	m_bWorking = !m_bWorking;
	// 通知游戏线程开始挂机或停止挂机
	NotifyMessage(RXJHMSG_AUTOWORK, m_bWorking ? 1 : 0);

	//ActionCall(1);
	//UseTheGoodsCall(GetGoodsIDforName("金创药(大)"));

	//float x = 2240.0;
	//float y = -420.0;
	//FindWalk(x, y);

	//CheckEntity();
	
	//::Sleep(200);
	//UseTheF1_F10Call_(2);
}

BOOL CCDialg_Main::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// 初始控件数据
	CString szShrotCut[10] = {" F1", " F2", " F3", " F4", " F5", " F6", " F7", " F8", " F9", " F10" };
	m_attackType.AddString("平砍");
	for (int i = 0; i < 10; i++)
	{
		m_attackType.AddString(szShrotCut[i]);
	}
	m_attackType.SetCurSel(0);

	Config& cfg = Config::GetConfig();
	for (auto& item : cfg.hpDrugs)
	{
		m_HPList.AddString(item);
		m_comboBuyList.AddString(item);
	}
	m_HPList.SetCurSel(5);

	for (auto& item : cfg.mpDrugs)
	{
		m_MPList.AddString(item);
		m_comboBuyList.AddString(item);
	}
	m_MPList.SetCurSel(3);
	m_comboBuyList.SetCurSel(0);

	for (auto& item : cfg.maps)
	{
		m_comboMap.AddString(item.szName);
		m_comboSupply.AddString(item.szName);
	}
	m_comboMap.SetCurSel(0);
	m_comboSupply.SetCurSel(0);

	// 通知游戏线程，设置窗口已创建
	NotifyMessage(RXJHMSG_INITIALIZE);

	return TRUE;
}

void CCDialg_Main::SetGameWnd(HWND a_hwnd)
{
	m_hGameWnd = a_hwnd;
}

Config CCDialg_Main::GetConfigData()
{
	Config cfg;
	m_cs.Lock();
	cfg = m_cfg;
	m_cs.Unlock();

	return cfg;
}

void CCDialg_Main::LoadConfigByName(const CString& a_roleName)
{
	// 加载配置

	// 通知更新并开始保护处理
	NotifyMessage(RXJHMSG_UPDATE_SETTING, 1);
}

LRESULT CCDialg_Main::OnMsgLoadConfig(WPARAM a_wparam, LPARAM a_lparam)
{
	char* pName = (char*)a_wparam;
	if (NULL != pName)
	{
		LoadConfigByName(pName);
	}
	return 1;
}

void CCDialg_Main::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMERID_PROTECT)
	{
		UpdateData();

		// 检测是否切换人物
		if (m_hGameWnd)
		{
			CString szName = m_playHelper.GetRoleName();

			char szPreName[64] = { 0 };
			::GetWindowTextA(m_hGameWnd, szPreName, 64);
			if (szPreName != szPreName)
			{
				OnClose();
				return;
			}
		}

		m_playHelper.Protect();
		/*CString szMapName = m_playHelper.GetCurMap();
		if (szMapName != m_szMapName)
		{
			m_szMapName = szMapName;
			UpdateData(FALSE);
		}*/
	}
	else if (nIDEvent == TIMERID_ATTACK)
	{
		m_playHelper.Work();
	}

	CDialog::OnTimer(nIDEvent);
}

void CCDialg_Main::ReadData()
{
	m_nHong = Read_RD(UserBaseAddress + UserHPOffset);
	m_nLan = Read_RD(UserBaseAddress + UserMPOffset);
	//DWORD dwBase = *((DWORD *)CordinateBaseAddress);
	//m_nX = *((int *)(dwBase + CordinateXOffset));
	//m_nY = *((int *)(dwBase + CordinateYOffset));
	//m_nY = 0 - m_nY;
	m_nX = Read_RD(Read_RD(CordinateBaseAddress) + CordinateXOffset);
	m_nY = Read_RD(Read_RD(CordinateBaseAddress) + CordinateYOffset);
	m_nY = 0 - m_nY;

	//char* pMapName = Read_RS(Read_RD(MapBaseAddress) + MapNameOffset);
	//m_szMapName = pMapName;

	UpdateData(FALSE);
}

void CCDialg_Main::AttackWork()
{
	DWORD dwSel = GetEntitySelID();
	DWORD dwSelNation = 0;
	if (ENTITY_NOTSEL_ID == dwSel)
	{
		dwSel = CheckEntity();
	}

	if (ENTITY_NOTSEL_ID != dwSel)
	{
		dwSelNation = Read_RD(EntityPropAddress + dwSel * 4);
		DWORD dwJueming = GetEntityJueming(dwSelNation);
		// 是否可以使用绝命技
		if (0 != dwJueming)
		{
			// 需要把绝命技能放在F8上
			UseTheF1_F10Call_(7);
		}
		else
		{
			// 普通攻击或技能
			ActionCall();
		}
	}
	//ActionCall(1);
	//UseTheGoodsCall(GetGoodsIDforName("金创药(大)"));
	//CheckEntity();
	//::Sleep(200);
	//UseTheF1_F10Call_(0);
}
void CCDialg_Main::ProtectWork()
{
	//ReadData();
	// 生命值小于设定值，加血默认按F2
	if (m_nHong < 300)
	{
		UseTheF1_F10Call_(1);
	}
	// 蓝小于设定值，加血默认按F3
	if (m_nLan < 100)
	{
		UseTheF1_F10Call_(2);
	}
}

void CCDialg_Main::SetPoint(POINT a_pt)
{
	m_cs.Lock();
	m_cfg.pt = a_pt;

	m_nX = a_pt.x;
	m_nY = a_pt.y;
	m_cs.Unlock();

	// 通知自己刷新界面
	PostMessageA(RXJHMSG_REFRESHUI);
	//UpdateData(FALSE);
}

LRESULT CCDialg_Main::OnMsgRefreshUI(WPARAM a_wparam, LPARAM a_lparam)
{
	UpdateData(FALSE);

	return 1;
}

void CCDialg_Main::OnGetCurrentPoint()
{
	// TODO: 在此添加控件通知处理程序代码

	NotifyMessage(RXJHMSG_GETPOINT);

}


void CCDialg_Main::OnApplyConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	
	m_cs.Lock();
	// 挂机设置
	POINT pt = { m_nX, m_nY };
	m_cfg.pt = pt;
	m_comboMap.GetWindowTextA(m_cfg.szWorkMap);
	int sel = m_attackType.GetCurSel();
	m_cfg.nAttackType = sel;
	m_cfg.nAttackRange = m_nAttackRange;
	m_cfg.bNearestPrior = m_bNearest;
	// 保护设置
	m_cfg.nProtectHP = m_nProtectHP;
	m_cfg.nProtentMP = m_nProtectMP;
	m_HPList.GetWindowTextA(m_cfg.szPriorHPDrug);
	m_MPList.GetWindowTextA(m_cfg.szPriorMPDrug);
	// 回城设置
	m_cfg.bCheckHPDrugs = m_bHPCounts;
	m_cfg.nMinHPDrugs = m_nHPCounts;
	m_cfg.bCheckMPDrugs = m_bMPCounts;
	m_cfg.nMinMPDrugs = m_nMPCounts;
	m_cfg.bCheckArrows = m_bArrows;
	m_cfg.nMinArrows = m_nArrowCounts;
	m_cfg.bCheckPackage = m_bPackageFull;
	m_comboSupply.GetWindowTextA(m_cfg.szSupplyMap);

	m_cs.Unlock();
	// 通知游戏线程更新设置
	NotifyMessage(RXJHMSG_UPDATE_SETTING);
}

void CCDialg_Main::NotifyMessage(UINT a_msg, WPARAM a_wparam, LPARAM a_lparam)
{
	::PostMessageA(m_hGameWnd, a_msg, a_wparam, a_lparam);
}

void CCDialg_Main::OnCbnDropdownHPList()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_HPList.AddString("test");
}


void CCDialg_Main::OnLoadConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CString szGoods;
	m_comboBuyList.GetWindowTextA(szGoods);
	EntityNPC npc(0x15da);
	npc.OpenTalk();
	//::Sleep(2000);
	npc.OpenShop();
	npc.BuyGoodsByName(szGoods, m_nBuyCount);
	//::Sleep(2000);
	npc.CloseShop();
	//::Sleep(2000);
	npc.CloseTalk();
	//npc.BuyGoodsByName(szGoods, m_nBuyCount);
}


void CCDialg_Main::OnAddBuyGoods()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CString szBuy;
	m_comboBuyList.GetWindowTextA(szBuy);
	CString szItem;
	szItem.Format("购买 %s: %d", szBuy, m_nBuyCount);

	CString* pData = new CString(szBuy);
	int nCount = m_listBuys.AddString(szItem);
	m_listBuys.SetItemData(nCount, (DWORD_PTR)pData);

	Config& cfg = Config::GetConfig();
	cfg.buys[szBuy] = m_nBuyCount;
}


void CCDialg_Main::OnModifyBuyItem()
{
	// TODO: 在此添加命令处理程序代码
	int nSel = m_listBuys.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString* pData = (CString*)m_listBuys.GetItemData(nSel);
		if (pData)
		{
			Config& cfg = Config::GetConfig();
			auto iter = cfg.buys.find(*pData);
			int nBuy = iter->second;
			CBuyCountDlg dlg(nBuy);
			dlg.DoModal();
			int nNewBuy = dlg.GetBuyCount();
			if (nNewBuy != nBuy)
			{
				m_listBuys.DeleteString(nSel);

				CString szItem;
				szItem.Format("购买 %s: %d", *pData, nNewBuy);
				m_listBuys.InsertString(nSel, szItem);
				m_listBuys.SetItemData(nSel, (DWORD_PTR)pData);

				cfg.buys[*pData] = nNewBuy;
			}
		}
	}
}


void CCDialg_Main::OnDeleteBuyItem()
{
	// TODO: 在此添加命令处理程序代码
	int nSel;
	nSel = m_listBuys.GetCurSel();
	if (nSel == CB_ERR)
	{
		MessageBox("请选中一个记录");
	}
	else
	{
		CString* pData = (CString*)m_listBuys.GetItemData(nSel);
		CString szName = *pData;
		if (pData)
		{
			delete pData;
			pData = NULL;
		}
		//删除listbox 中的记录
		m_listBuys.DeleteString(nSel);

		Config& cfg = Config::GetConfig();
		cfg.buys.erase(szName);
	}
}


void CCDialg_Main::OnDblclkListBuy()
{
	// TODO: 在此添加控件通知处理程序代码
	OnModifyBuyItem();
}
