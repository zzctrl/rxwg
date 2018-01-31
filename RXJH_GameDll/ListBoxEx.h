#pragma once
#include "afxwin.h"

class CListBoxEx :
	public CListBox
{
public:
	CListBoxEx();
	~CListBoxEx();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

