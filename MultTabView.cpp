// MultTabView.cpp : implementation file
//

#include "stdafx.h"
#include "MultTabView.h"


// CMultTabView

IMPLEMENT_DYNCREATE(CMultTabView, CView)

CMultTabView::CMultTabView()
	: m_bIsReady(FALSE)
{
}

CMultTabView::~CMultTabView()
{
}

BEGIN_MESSAGE_MAP(CMultTabView, CView)
	ON_WM_MOUSEACTIVATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CMultTabView::OnChangeActiveTab)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMultTabView drawing

void CMultTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}



int CMultTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CMultTabView::OnDestroy()
{
	RemoveAllTabCtrl();
	CView::OnDestroy();
}

void CMultTabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	AdjustRect();
}

void CMultTabView::AdjustRect()
{
	// Tab control should cover a whole client area:
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	int width=m_wndTabsList.size()==0? rect.Width():rect.Width()/m_wndTabsList.size()-2;
	int i=0;
	for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++i,++list_Iter)
	{
		(*list_Iter)->m_wndTabs->SetWindowPos(NULL, width*i+2, 0, width-2, rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

// CMultTabView diagnostics

#ifdef _DEBUG
void CMultTabView::AssertValid() const
{
	CView::AssertValid();
}

void CMultTabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CMultTabView message handlers

void CMultTabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_bIsReady = TRUE;

	//选择激活那个view
	//for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
	//	OnChangeActiveTab((*list_Iter)->m_nFirstActiveTab, (LPARAM)(*list_Iter)->m_wndTabs);
}

class CInternalTabView : public CView
{
	friend class CMultTabView;
};

int CMultTabView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CView* pCurrView = NULL;
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
		{
			CRect rect;
			(*list_Iter)->m_wndTabs->GetWindowRect(&rect);
			ScreenToClient(&rect);
			if(rect.PtInRect(point))
				pCurrView = GetActiveView((*list_Iter)->m_wndTabs);
		}
	}

	if (pCurrView == NULL)
	{
		return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
		return nResult;   // frame does not want to activate

	CFrameWnd* pParentFrame = AFXGetParentFrame(this);
	if (pParentFrame != NULL)
	{
		// eat it if this will cause activation
		ASSERT(pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame));

		// either re-activate the current view, or set this view to be active
		CView* pView = pParentFrame->GetActiveView();
		HWND hWndFocus = ::GetFocus();
		if (pView == pCurrView && pCurrView->m_hWnd != hWndFocus && !::IsChild(pCurrView->m_hWnd, hWndFocus))
		{
			// re-activate this view
			((CInternalTabView*)pCurrView)->OnActivateView(TRUE, pCurrView, pCurrView);
		}
		else
		{
			// activate this view
			pParentFrame->SetActiveView(pCurrView);
		}
	}

	return nResult;
}

LRESULT CMultTabView::OnChangeActiveTab(WPARAM wp, LPARAM lp)
{
	CMFCBaseTabCtrl* ptabctrl = (CMFCBaseTabCtrl*)lp;

	if (!m_bIsReady)
	{
		for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
		{
			if(ptabctrl==(*list_Iter)->m_wndTabs)
			{
				(*list_Iter)->m_nFirstActiveTab = (int) wp;
				break;
			}
		}
		return 0;
	}

	CFrameWnd* pFrame = AFXGetParentFrame(this);
	ASSERT_VALID(pFrame);

	int iTabNum = (int) wp;
	if (iTabNum >= 0)
	{
		CView* pView = DYNAMIC_DOWNCAST(CView, ptabctrl->GetTabWnd(iTabNum));
		ASSERT_VALID(pView);

		pFrame->SetActiveView(pView);

		OnActivateView(pView);
	}
	else
	{
		pFrame->SetActiveView(NULL);

		OnActivateView(NULL);
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------
CMFCTabCtrl* CMultTabView::AddTabCtrl(int TabIndex)
{
	MultTabViewInfo* tabinfo = new MultTabViewInfo(TabIndex);

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	// Create tabs window:
	if ((tabinfo->m_wndTabs!=NULL) && !tabinfo->m_wndTabs->Create(IsScrollBar()? CMFCTabCtrl::STYLE_FLAT_SHARED_HORZ_SCROLL : CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create tab window\n");
		return NULL;      // fail to create
	}
	tabinfo->m_wndTabs->SetFlatFrame();
	tabinfo->m_wndTabs->SetTabBorderSize(3);
	tabinfo->m_wndTabs->AutoDestroyWindow(FALSE);
	tabinfo->m_wndTabs->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
	tabinfo->m_wndTabs->SetLocation(CMFCTabCtrl::LOCATION_TOP);
	tabinfo->m_wndTabs->EnableAutoColor();

	m_wndTabsList.push_back(tabinfo);
	return tabinfo->m_wndTabs;
}

void CMultTabView::RemoveTabCtrl(int TabIndex)
{
	for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
	{
		if((*list_Iter)->m_TabIndex==TabIndex)
		{
			for(int i=0; i<(*list_Iter)->m_wndTabs->GetTabsNum(); i++)
				RemoveView((*list_Iter)->m_wndTabs,i);
			(*list_Iter)->m_wndTabs->SendMessage(WM_CLOSE);
			delete (*list_Iter);
			m_wndTabsList.erase(list_Iter);
			break;
		}
	}
}

void CMultTabView::RemoveAllTabCtrl()
{
	for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
	{
		for(int i=0; i<(*list_Iter)->m_wndTabs->GetTabsNum(); i++)
			RemoveView((*list_Iter)->m_wndTabs,i);
		(*list_Iter)->m_wndTabs->SendMessage(WM_CLOSE);
		delete (*list_Iter);
	}
	m_wndTabsList.erase(m_wndTabsList.begin(), m_wndTabsList.end());
	m_wndTabsList.clear();
}

CMFCTabCtrl* CMultTabView::GetTabCtrl(int TabIndex)
{
	CMFCTabCtrl* ctrl=NULL;
	for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
	{
		if((*list_Iter)->m_TabIndex==TabIndex)
		{
			ctrl=(*list_Iter)->m_wndTabs;
			break;
		}
	}
	return ctrl;
}

//-----------------------------------------------------------------------------------------------------------------------
int CMultTabView::AddView(CMFCTabCtrl* m_wndTabs, CRuntimeClass* pViewClass, const CString& strViewLabel, int iIndex /*= -1*/, CCreateContext* pContext/* = NULL*/)
{
	ASSERT_VALID(this);
	ENSURE(pViewClass != NULL);
	ENSURE(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CView)));

	CView* pView = DYNAMIC_DOWNCAST(CView, pViewClass->CreateObject());
	ASSERT_VALID(pView);

	if (!pView->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), m_wndTabs, (UINT) -1, pContext))
	{
		TRACE1("CMultTabView:Failed to create view '%s'\n", pViewClass->m_lpszClassName);
		return -1;
	}

	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		ASSERT_VALID(pDoc);

		BOOL bFound = FALSE;
		for (POSITION pos = pDoc->GetFirstViewPosition(); !bFound && pos != NULL;)
		{
			if (pDoc->GetNextView(pos) == pView)
			{
				bFound = TRUE;
			}
		}

		if (!bFound)
		{
			pDoc->AddView(pView);
		}
	}

	m_wndTabs->InsertTab(pView, strViewLabel, iIndex);

	int nTabs = m_wndTabs->GetTabsNum();
	return nTabs - 1;
}

BOOL CMultTabView::RemoveView(CMFCTabCtrl* m_wndTabs, int iTabNum)
{
	ASSERT_VALID(this);
	return m_wndTabs->RemoveTab(iTabNum);
}

BOOL CMultTabView::SetActiveView(CMFCTabCtrl* m_wndTabs, int iTabNum)
{
	ASSERT_VALID(this);
	return m_wndTabs->SetActiveTab(iTabNum);
}

CView* CMultTabView::GetActiveView(CMFCTabCtrl* m_wndTabs) const
{
	ASSERT_VALID(this);

	int iActiveTab = m_wndTabs->GetActiveTab();
	if (iActiveTab < 0)
	{
		return NULL;
	}

	return DYNAMIC_DOWNCAST(CView, m_wndTabs->GetTabWnd(iActiveTab));
}

CView* CMultTabView::GetView(CMFCTabCtrl* m_wndTabs, int Index) const
{
	return DYNAMIC_DOWNCAST(CView, m_wndTabs->GetTabWnd(Index));
}

int CMultTabView::FindTab(CMFCTabCtrl* m_wndTabs, HWND hWndView) const
{
	ASSERT_VALID(this);

	for (int i = 0; i < m_wndTabs->GetTabsNum(); i++)
	{
		if (m_wndTabs->GetTabWnd(i)->GetSafeHwnd() == hWndView)
		{
			return i;
		}
	}

	return -1;
}
