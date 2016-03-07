#pragma once


// CToolTipListCtrl
class CToolTipListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CToolTipListCtrl)

public:
	CToolTipListCtrl();
	virtual ~CToolTipListCtrl();

	void SetToolTipInfo(int subitem, CString& desstr)
	{
		m_ToolTip_SubIndex = subitem;
		m_ToolTip_DesStr = desstr;
	}

#if defined(_WIN64)
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#else
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#endif
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg BOOL CreateToolTip(CString sItemName,const CString &sDesc, const CRect& ItemRc);

	virtual BOOL PreTranslateMessage(MSG* pMsg); 

	virtual int OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
private:
	CToolTipCtrl	*m_pToolTip;
	int m_ToolTip_SubIndex;
	CString m_ToolTip_DesStr;
};