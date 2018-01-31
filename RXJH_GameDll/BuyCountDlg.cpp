// BuyCountDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RXJH_GameDll.h"
#include "BuyCountDlg.h"
#include "afxdialogex.h"


// CBuyCountDlg 对话框

IMPLEMENT_DYNAMIC(CBuyCountDlg, CDialogEx)

CBuyCountDlg::CBuyCountDlg(DWORD nBuy, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_nBuy(nBuy)
{

}

CBuyCountDlg::~CBuyCountDlg()
{
}

void CBuyCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nBuy);
}


BEGIN_MESSAGE_MAP(CBuyCountDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBuyCountDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBuyCountDlg 消息处理程序


void CBuyCountDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CDialogEx::OnOK();
}

DWORD CBuyCountDlg::GetBuyCount()
{
	return m_nBuy;
}
