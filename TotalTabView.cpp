// TotalTabView.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "TotalTabView.h"

#include "EmptyFileView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EMPTY_FILE_INDEX 1
//#define HSF_INDEX 2
// CTotalTabView

IMPLEMENT_DYNCREATE(CTotalTabView, CMultTabView)

CTotalTabView::CTotalTabView()
{

}

CTotalTabView::~CTotalTabView()
{
}

BEGIN_MESSAGE_MAP(CTotalTabView, CMultTabView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CMultTabView::OnChangeActiveTab)
END_MESSAGE_MAP()


// CTotalTabView diagnostics

#ifdef _DEBUG
void CTotalTabView::AssertValid() const
{
	CMultTabView::AssertValid();
}

void CTotalTabView::Dump(CDumpContext& dc) const
{
	CMultTabView::Dump(dc);
}

CFileManagerDoc* CTotalTabView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileManagerDoc)));
	return (CFileManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CTotalTabView message handlers


int CTotalTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMultTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//读取注册表
	LoadRegedits();

	CMFCTabCtrl* m_sbqwndTabs = AddTabCtrl(EMPTY_FILE_INDEX);
	if(m_sbqwndTabs!=NULL)
	{
		
		AddView(m_sbqwndTabs, RUNTIME_CLASS(CEmptyFileView), theApp.LoadString(IDS_EmptyFileManger), 0);
		//AddView(m_sbqwndTabs, RUNTIME_CLASS(CSpectrumDispView), SPECTRUM_DISPLAY_NAME[theApp.language], 1);
		//AddView(m_sbqwndTabs, RUNTIME_CLASS(CLisaruDispView), SBQ_LISSAJOUS_DISPLAY_NAME[theApp.language], 2);
	}
	return 0;
}

void CTotalTabView::OnInitialUpdate()
{
	CMultTabView::OnInitialUpdate();
	ActiveEmptyFileManagerTabCtrl();
}

//-------------------------------------------------------------------------------------------
void CTotalTabView::ActiveEmptyFileManagerTabCtrl()
{
	//激活界面
	for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
	{
		if((*list_Iter)->m_TabIndex==EMPTY_FILE_INDEX)  
			OnChangeActiveTab((*list_Iter)->m_nFirstActiveTab, (LPARAM)(*list_Iter)->m_wndTabs);
	}
}

void CTotalTabView::ShowCtrlPanel(bool en)
{
	/*for(auto list_Iter=m_wndTabsList.begin(); list_Iter!=m_wndTabsList.end(); ++list_Iter)
	{
		if((*list_Iter)->m_TabIndex==SBQ_INDEX)
		{
			for(int i=0;i<3;i++)
			{
				CWnd* wnd = (*list_Iter)->m_wndTabs->GetTabWnd(i);
				CBasicDispView* pview = dynamic_cast<CBasicDispView*>(wnd);
				if(pview!=NULL)
					pview->ShowCtrlPanel(en);
			}
		}
	}*/
}

void CTotalTabView::LoadRegedits()
{
	/*CString base = theApp.GetRegistryBase();
	theApp.SetRegistryBase(APP_REGISTBASE);
	m_hsfdisp_view=theApp.GetInt("Hsf_Disp_View",m_hsfdisp_view);
	theApp.SetRegistryBase(base);*/
}

void CTotalTabView::SaveRegedits()
{
	/*CString base = theApp.GetRegistryBase();
	theApp.SetRegistryBase(APP_REGISTBASE);
	theApp.WriteInt("Hsf_Disp_View",m_hsfdisp_view);
	theApp.SetRegistryBase(base);

	//枚举view
	CMFCTabCtrl* wndTabs = GetTabCtrl(SBQ_INDEX);
	if(wndTabs!=NULL)
	{
		for(int k=0;k<3;k++)
		{
			CBasicDispView* pview = dynamic_cast<CBasicDispView*>( GetView(wndTabs, k));
			if(pview!=NULL)
				pview->SaveRegedits();
		}
	}

	wndTabs = GetTabCtrl(HSF_INDEX);
	if(wndTabs!=NULL)
	{
		CHsfDispView* pview = dynamic_cast<CHsfDispView*>( GetView(wndTabs, 0));
		if(pview!=NULL)
			pview->SaveRegedits();
	}*/
}

