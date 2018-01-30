// CDialg_Main.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "CDialg_Main.h"
#include "afxdialogex.h"
#include "CommonRead.h"
#include "AddressData.h"
#include "EntityPlayer.h"


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
	, m_szMapName(_T(""))
	, m_nAttackRange(150)
	, m_nProtectHP(150)
	, m_nProtectMP(100)
	, m_bNearest(TRUE)
{
	m_bWorking = false;
}

CCDialg_Main::~CCDialg_Main()
{
}

void CCDialg_Main::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nHong);
	DDX_Text(pDX, IDC_EDIT2, m_nLan);
	DDX_Text(pDX, IDC_EDIT4, m_nX);
	DDX_Text(pDX, IDC_EDIT3, m_nY);
	DDX_Control(pDX, IDC_BUTTON, m_btnWork);
	DDX_Text(pDX, IDC_EDIT8, m_szMapName);
	DDX_Text(pDX, IDC_EDIT5, m_nAttackRange);
	DDX_Text(pDX, IDC_EDIT6, m_nProtectHP);
	DDX_Text(pDX, IDC_EDIT7, m_nProtectMP);
	DDX_Check(pDX, IDC_CHECK1, m_bNearest);
	DDX_Control(pDX, IDC_COMBO1, m_attackType);
	DDX_Control(pDX, IDC_COMBO2, m_HPList);
	DDX_Control(pDX, IDC_COMBO3, m_MPList);
}


BEGIN_MESSAGE_MAP(CCDialg_Main, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON, &CCDialg_Main::OnBnClickedButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CCDialg_Main::OnGetCurrentPoint)
	ON_BN_CLICKED(IDC_BUTTON3, &CCDialg_Main::OnApplyConfig)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CCDialg_Main::OnCbnDropdownHPList)
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
		m_playHelper.Start();
		SetTimer(TIMERID_ATTACK, 800, NULL);
		m_btnWork.SetWindowTextA("停止挂机");
	}
	else
	{
		m_playHelper.Stop();
		KillTimer(TIMERID_ATTACK);
		m_btnWork.SetWindowTextA("开始挂机");
	}
	m_bWorking = !m_bWorking;
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

	// 初始化地址数据
	InitAddress();

	SetTimer(TIMERID_PROTECT, 200, NULL);
	
	// 初始控件数据
	CString szShrotCut[10] = {" F1", " F2", " F3", " F4", " F5", " F6", " F7", " F8", " F9", " F10" };
	m_attackType.AddString("平砍");
	for (int i = 0; i < 10; i++)
	{
		m_attackType.AddString(szShrotCut[i]);
	}
	m_attackType.SetCurSel(0);

	m_HPList.AddString("金创药(小)");
	m_HPList.AddString("金创药(中)");
	m_HPList.AddString("金创药(大)");
	m_HPList.AddString("金创药(特)");
	m_HPList.AddString("秘制金创药");
	m_HPList.SetCurSel(4);

	m_MPList.AddString("人参");
	m_MPList.AddString("野山参");
	m_MPList.AddString("雪原参");
	m_MPList.AddString("秘制雪原参");
	m_MPList.SetCurSel(3);

	return TRUE;
}

void CCDialg_Main::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMERID_PROTECT)
	{
		UpdateData();

		m_playHelper.Protect();
		CString szMapName = m_playHelper.GetCurMap();
		if (szMapName != m_szMapName)
		{
			m_szMapName = szMapName;
			UpdateData(FALSE);
		}
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

	char* pMapName = Read_RS(Read_RD(MapBaseAddress) + MapNameOffset);
	m_szMapName = pMapName;

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
	ReadData();
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

void CCDialg_Main::OnGetCurrentPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	POINT pt = m_playHelper.GetCurPoint();
	m_playHelper.SetWorkPoint(pt);

	m_nX = pt.x;
	m_nY = pt.y;
	
	UpdateData(FALSE);
}


void CCDialg_Main::OnApplyConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	POINT pt = { m_nX, m_nY };
	m_playHelper.SetWorkPoint(pt);
	m_playHelper.SetWorkRange(m_nAttackRange);
	m_playHelper.SetNearestPrior(m_bNearest);

	m_playHelper.SetProtectHP(m_nProtectHP);
	m_playHelper.SetProtectMP(m_nProtectMP);

	int sel = m_attackType.GetCurSel();
	m_playHelper.SetAttackIndex(sel);

	CString szName;
	m_HPList.GetWindowTextA(szName);
	m_playHelper.SetPriorHPDrug(szName);

	m_MPList.GetWindowTextA(szName);
	m_playHelper.SetPriorMPDrug(szName);

}


void CCDialg_Main::OnCbnDropdownHPList()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_HPList.AddString("test");
}
