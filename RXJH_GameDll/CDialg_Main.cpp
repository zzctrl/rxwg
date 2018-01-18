// CDialg_Main.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "CDialg_Main.h"
#include "afxdialogex.h"
#include "CommonRead.h"
#include "AddressData.h"


// CCDialg_Main 对话框

IMPLEMENT_DYNAMIC(CCDialg_Main, CDialog)

CCDialg_Main::CCDialg_Main(CWnd* pParent /*=NULL*/)
	: CDialog(CCDialg_Main::IDD, pParent)
	, m_nHong(0)
	, m_nLan(0)
	, m_nX(0)
	, m_nY(0)
{

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
	// TODO: 在此添加控件通知处理程序代码
	//UseTheGoodsCall(GetGoodsIDforName("金创药(大)"));
	CheckEntity();
}

#define TIMERID_REFRESH			0x1000

BOOL CCDialg_Main::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 初始化地址数据
	InitAddress();

	SetTimer(TIMERID_REFRESH, 500, NULL);

	return TRUE;
}

void CCDialg_Main::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIMERID_REFRESH)
	{
		ReadData();
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

static const DWORD s_addrHong = 0x02AEBCB8;
static const DWORD s_addrLan = 0x02AEBCBC;
static const DWORD s_addrBase = 0x02D37970;
static const DWORD s_offsetX = 0x36C;
static const DWORD s_offsetY = 0x370;

void CCDialg_Main::ReadData()
{
	//m_nHong = *((int *)s_addrHong);
	//m_nLan = *((int *)s_addrLan);
	m_nHong = Read_RD(UserBaseAddress + UserHPOffset);
	m_nLan = Read_RD(UserBaseAddress + UserMPOffset);
	//DWORD dwBase = *((DWORD *)CordinateBaseAddress);
	//m_nX = *((int *)(dwBase + CordinateXOffset));
	//m_nY = *((int *)(dwBase + CordinateYOffset));
	//m_nY = 0 - m_nY;
	m_nX = Read_RD(Read_RD(CordinateBaseAddress) + CordinateXOffset);
	m_nY = Read_RD(Read_RD(CordinateBaseAddress) + CordinateYOffset);
	m_nY = 0 - m_nY;
}