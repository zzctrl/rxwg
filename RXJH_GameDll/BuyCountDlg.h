#pragma once


// CBuyCountDlg 对话框

class CBuyCountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBuyCountDlg)

public:
	CBuyCountDlg(DWORD nBuy, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBuyCountDlg();


	DWORD GetBuyCount();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_nBuy;
	afx_msg void OnBnClickedOk();
};
