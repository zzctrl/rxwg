#pragma once


// CCDialg_Main 对话框
#include "Resource.h"
#include "Funtion.h"
#include "afxwin.h"
#include "PlayHelper.h"

class CCDialg_Main : public CDialog
{
	DECLARE_DYNAMIC(CCDialg_Main)

public:
	CCDialg_Main(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCDialg_Main();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void ReadData();

	void AttackWork();
	void ProtectWork();

	int m_nHong;
	int m_nLan;
	int m_nX;
	int m_nY;

	bool m_bWorking;

	PlayHelper m_playHelper;
public:
	CButton m_btnWork;
	CString m_szMapName;
	DWORD m_nAttackRange;
	DWORD m_nProtectHP;
	DWORD m_nProtectMP;
	BOOL m_bNearest;
	CComboBox m_attackType;
	CComboBox m_HPList;
	CComboBox m_MPList;
	
	afx_msg void OnGetCurrentPoint();
	afx_msg void OnApplyConfig();
	afx_msg void OnCbnDropdownHPList();
};
