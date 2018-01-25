// CDialg_Main.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "CDialg_Main.h"
#include "afxdialogex.h"
#include "CommonRead.h"
#include "AddressData.h"


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
	, m_nHongPer(50)
	, m_nLanPer(50)
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
	DDX_Text(pDX, IDC_EDIT6, m_nHongPer);
	DDX_Text(pDX, IDC_EDIT7, m_nLanPer);
}


BEGIN_MESSAGE_MAP(CCDialg_Main, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON, &CCDialg_Main::OnBnClickedButton)
	ON_WM_TIMER()
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


void CCDialg_Main::OnBnClickedButton()
{
	CheckNPC(0x15a3);
	OpenNPCTalk(0x15a3);
	return;
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bWorking)
	{
		SetTimer(TIMERID_ATTACK, 1000, NULL);
		m_btnWork.SetWindowTextA("停止挂机");
	}
	else
	{
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

	return TRUE;
}

void CCDialg_Main::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMERID_PROTECT)
	{
		ProtectWork();
	}
	else if (nIDEvent == TIMERID_ATTACK)
	{
		AttackWork();
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