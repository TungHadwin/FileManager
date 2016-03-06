#pragma once
#include <list>


class MultTabViewInfo
{
	MultTabViewInfo()
		: m_wndTabs(NULL)
		, m_TabIndex(0)
		, m_nFirstActiveTab(-1)
	{
	};
public:
	MultTabViewInfo(int TabIndex)
		: m_TabIndex(TabIndex)
		, m_nFirstActiveTab(-1)
	{
		m_wndTabs=new CMFCTabCtrl();
	};

	virtual ~MultTabViewInfo()
	{
		delete m_wndTabs;
		m_wndTabs=NULL;
	};

	MultTabViewInfo(const MultTabViewInfo& copy)
	{
		m_wndTabs = copy.m_wndTabs;
		m_nFirstActiveTab = copy.m_nFirstActiveTab;
		m_TabIndex = copy.m_TabIndex;
	}

	MultTabViewInfo& operator=(const MultTabViewInfo& copy)
	{
		m_wndTabs = copy.m_wndTabs;
		m_nFirstActiveTab = copy.m_nFirstActiveTab;
		m_TabIndex = copy.m_TabIndex;
		return *this;
	}
public:
	CMFCTabCtrl*	m_wndTabs;
	int				m_nFirstActiveTab;
	int				m_TabIndex;
};

// CMultTabView view

class CMultTabView : public CView
{
	DECLARE_DYNCREATE(CMultTabView)

protected:
	CMultTabView();           // protected constructor used by dynamic creation
	virtual ~CMultTabView();

public:
	CMFCTabCtrl* AddTabCtrl(int TabIndex);
	void RemoveTabCtrl(int TabIndex);
	void RemoveAllTabCtrl();
	CMFCTabCtrl* GetTabCtrl(int TabIndex);

	int AddView(CMFCTabCtrl* m_wndTabs, CRuntimeClass* pViewClass, const CString& strViewLabel, int iIndex = -1, CCreateContext* pContext = NULL);
	BOOL RemoveView(CMFCTabCtrl* m_wndTabs, int iTabNum);

	BOOL SetActiveView(CMFCTabCtrl* m_wndTabs, int iTabNum);
	CView* GetActiveView(CMFCTabCtrl* m_wndTabs) const; 
	CView* GetView(CMFCTabCtrl* m_wndTabs, int Index) const; 

	int FindTab(CMFCTabCtrl* m_wndTabs, HWND hWndView) const;
	
public:
	virtual void OnInitialUpdate();

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnChangeActiveTab(WPARAM,LPARAM);

	virtual void OnActivateView(CView* /*pView*/){};
	virtual BOOL IsScrollBar() const { return FALSE; }

	virtual void AdjustRect();
protected:
	std::list<MultTabViewInfo*> m_wndTabsList;
	BOOL    m_bIsReady;
};


