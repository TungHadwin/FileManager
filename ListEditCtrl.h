#pragma once
// ListEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl window

class CListEditCtrl : public CEdit
{
// Construction
public:
	CListEditCtrl(int iItem, int iSubItem, CString sInitText);
// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListEditCtrl(){};
private:	
	int m_iItem;
	int m_iSubItem;	
	CString m_strInitText;
	BOOL  m_bVK_ESCAPE;
	// Generated message map functions
protected:
	//{{AFX_MSG(CListEditCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
