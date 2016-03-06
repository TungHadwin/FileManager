// ToolTipListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "ToolTipListCtrl.h"

IMPLEMENT_DYNAMIC(CToolTipListCtrl, CMFCListCtrl)

CToolTipListCtrl::CToolTipListCtrl()
: m_pToolTip(NULL)
{

}

CToolTipListCtrl::~CToolTipListCtrl()
{
	if(m_pToolTip!=NULL)
		delete m_pToolTip;
}

BEGIN_MESSAGE_MAP(CToolTipListCtrl, CMFCListCtrl)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()



// CTipGridListCtrlEx message handlers
#if defined(_WIN64)
INT_PTR CToolTipListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
#else
int CToolTipListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
#endif
{
	LVHITTESTINFO lvhitTestInfo;  
  
    lvhitTestInfo.pt    = point;  
  
    int nItem = ListView_SubItemHitTest(this->m_hWnd,&lvhitTestInfo);  
    UINT nFlags =   lvhitTestInfo.flags;  
  
    //nFlags is 0 if the SubItemHitTest fails  
    //Therefore, 0 & <anything> will equal false  
    //?????if(nItem!=-1 && (nFlags & m_wHitMask))  
	if(nItem!=-1) 
    {  
        CRect ItemRc;  
        GetItemRect(nItem,&ItemRc, LVIR_BOUNDS);  
  
        pTI->hwnd = m_hWnd;  
        pTI->uId = (lvhitTestInfo.iSubItem<<16) | (nItem);  
        pTI->lpszText = LPSTR_TEXTCALLBACK;  
        pTI->rect = *((RECT*)ItemRc);  
  
        //By returning a unique value per listItem,  
        //we ensure that when the mouse moves over another list item,  
        //the tooltip will change  
        return (int)pTI->uId;   
    }
    //Otherwise, we aren't interested, so let the message propagate  
    return -1;  
}

//here we supply the text for the item 
BOOL CToolTipListCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    ENSURE(pNMHDR != NULL);  
  
    if (CMFCPopupMenu::GetActiveMenu() != NULL)  
    {  
        return FALSE;  
    }  
  
    // dont care on id, do hit test  
    LVHITTESTINFO lvhitTestInfo;  
  
    //Get the mouse position  
    const MSG* pMessage;  
    pMessage = GetCurrentMessage();  
    ASSERT ( pMessage );  
    lvhitTestInfo.pt = pMessage->pt; //Get the point from the message  
    ScreenToClient( &lvhitTestInfo.pt ); //Convert the point's coords to be relative to this control  
      
    CRect ItemRc;  
    CString sItemName,sDesc;  
  
    int nItem = ListView_SubItemHitTest(this->m_hWnd,&lvhitTestInfo);  
  
    //nFlags is 0 if the SubItemHitTest fails  
    //if( nItem!=-1 && lvhitTestInfo.flags & m_wHitMask )  
	if( nItem!=-1 )  
    {  
        //int nSubItem = lvhitTestInfo.iSubItem;  
		sDesc=GetItemText(nItem, m_ToolTip_SubIndex);
        GetItemRect(nItem, &ItemRc, LVIR_BOUNDS);  
    }  
    return CreateToolTip(sDesc, m_ToolTip_DesStr, ItemRc );  
}

BOOL CToolTipListCtrl::CreateToolTip(CString sItemName,const CString &sDesc, const CRect& ItemRc)
{
	if(sDesc.IsEmpty())
		return FALSE;
	CTooltipManager::CreateToolTip(m_pToolTip,this,AFX_TOOLTIP_TYPE_DEFAULT);

	if(m_pToolTip->GetSafeHwnd()!=NULL)
	{
		CMFCToolTipCtrl* pToolTip=DYNAMIC_DOWNCAST(CMFCToolTipCtrl,m_pToolTip);
		if(pToolTip!=NULL)
		{
			CMFCToolTipInfo Params; 
			CMFCVisualManager::GetInstance()->GetToolTipInfo(Params);
            Params.m_bVislManagerTheme=TRUE;
			Params.m_bDrawDescription=TRUE;
			Params.m_nMaxDescrWidth=1000;
			pToolTip->SetParams(&Params);
			//pToolTip->SetDelayTime(TTDT_AUTOPOP,1000);
			//pToolTip->SetDelayTime(TTDT_INITIAL,100);
			//pToolTip->SetDelayTime(TTDT_RESHOW,1000);
			pToolTip->AddTool(this,sItemName,ItemRc,1);//???
            pToolTip->SetDescription(sDesc);
			SetToolTips(pToolTip);
			EnableTrackingToolTips(TRUE);
            pToolTip->Activate(TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

void CToolTipListCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CMFCListCtrl::PreSubclassWindow();

	EnableToolTips(TRUE);
}

BOOL CToolTipListCtrl::PreTranslateMessage(MSG* pMsg)   
{  
    //if( pMsg->message == TTN_SHOW ||  
    //    pMsg->message == TTN_GETDISPINFOW ||  
    //    pMsg->message == TTN_GETDISPINFOA )  
    //{  
    //    pMsgpMsg = pMsg; // tried to catch show/info message to no avail!  
    //}  
  
	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
		if (m_pToolTip!=NULL && m_pToolTip->GetSafeHwnd() != NULL)  
		{  
			m_pToolTip->RelayEvent(pMsg);  
		}  
		break;
	}
    return CMFCListCtrl::PreTranslateMessage(pMsg);  
}  
