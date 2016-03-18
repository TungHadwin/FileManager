// TempCleanerGrid.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "TempCleanerGrid.h"
#include "DeleteReadOnlyNoticeDlg.h"

// CTempCleanerGrid

IMPLEMENT_DYNAMIC(CTempCleanerGrid, CSuperGridCtrl)

CTempCleanerGrid::CTempCleanerGrid()
{
	SetToolTipInfo(1, theApp.LoadString(IDS_ListCtrl_ToolTip_DesString));
}

CTempCleanerGrid::~CTempCleanerGrid()
{
}


BEGIN_MESSAGE_MAP(CTempCleanerGrid, CSuperGridCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
END_MESSAGE_MAP()



// CTempCleanerGrid message handlers
void CTempCleanerGrid::InitializeGrid(void)
{
	/////////////////////
	//remember this	
	///////////////////

	//associate imagelist with listviewctrl, 
	//this imagelist is only for the first column re: the tree column	
	if(!m_image.Create(IDB_FOLDERS,16,1,RGB(0, 255, 255)))
		return ;

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return ;

	/*CImageList imageSubItem;
	if(!imageSubItem.Create(IDB_SUBITEMS,16,1,RGB(0, 255, 255)))
		return ;

    if(!SetSubItemImageList(&imageSubItem))
		return ;*/

	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SHOWSELALWAYS| LVS_EX_CHECKBOXES);

	//插入列
	InsertColumn(0, theApp.LoadString(IDS_FileName), LVCFMT_LEFT, 128);        
	InsertColumn(1, theApp.LoadString(IDS_FullPath), LVCFMT_LEFT, 138); 
	InsertColumn(2, theApp.LoadString(IDS_FileSize), LVCFMT_RIGHT, 80); 
	InsertColumn(3, theApp.LoadString(IDS_CreateTime), LVCFMT_RIGHT, 80); 
	InsertColumn(4, theApp.LoadString(IDS_AccessTime), LVCFMT_RIGHT, 80); 
	InsertColumn(5, theApp.LoadString(IDS_WriteTime), LVCFMT_RIGHT, 80); 
	InsertColumn(6, theApp.LoadString(IDS_SystemFile), LVCFMT_RIGHT, 80); 
	InsertColumn(7, theApp.LoadString(IDS_ReadOnly), LVCFMT_RIGHT, 80);
}

void CTempCleanerGrid::AdjustListColumnWidth()
{
	if(GetSafeHwnd())
	{
		CRect rect;
		GetWindowRect(rect);
		ScreenToClient(rect);

		SetColumnWidth(0,(rect.Width()-60*2-120*3-80*2)/3);
		SetColumnWidth(1,(rect.Width()-60*2-120*3-80*2)*2/3-4-8);
		SetColumnWidth(2,60);//size
		SetColumnWidth(3,120);
		SetColumnWidth(4,120);
		SetColumnWidth(5,120);
		SetColumnWidth(6,80);
		SetColumnWidth(7,80);
	}
}

void CTempCleanerGrid::ListFolders(std::list<TempCleanerInfo*>& cleaner_list, bool m_check_system, bool m_check_readonly, CString& Yes, CString& No)
{
	DeleteAll();

	for (auto cleaner_list_Iter = cleaner_list.begin(); cleaner_list_Iter != cleaner_list.end(); ++cleaner_list_Iter) 
	{
		if((*cleaner_list_Iter)->m_check)
		{
			//Create root item
			CItemInfo* lp = new CItemInfo();

			int pos = (*cleaner_list_Iter)->m_folder_path.ReverseFind('\\');
			CString str = (*cleaner_list_Iter)->m_folder_path.Right((*cleaner_list_Iter)->m_folder_path.GetLength()-pos-1);
			if(str.IsEmpty())
			{
				pos = (*cleaner_list_Iter)->m_folder_path.Find('\\');
				str = (*cleaner_list_Iter)->m_folder_path.Left(pos-1);
			}
			lp->SetItemText(str+'('+(*cleaner_list_Iter)->m_ext+')'); //add item text
			lp->AddSubItemText((*cleaner_list_Iter)->m_folder_path); //add subitem text // 0 zero based subitems...
			CTreeItem * pRoot = InsertRootItem(lp);//previous on N.Y.P.D we call it CreateTreeCtrl(lp)
			if( pRoot == NULL )
				return;

			//int empty_files_add_index=0;
			for (auto list_Iter = (*cleaner_list_Iter)->m_file_list.begin(); list_Iter != (*cleaner_list_Iter)->m_file_list.end(); ++list_Iter) 
			{
				if( ((*list_Iter)->IsSystem&&(!m_check_system)) || ((*list_Iter)->IsReadOnly&&(!m_check_readonly)) )
				{
					(*list_Iter)->IsDisplay = false;
					continue;
				}

				(*list_Iter)->IsDisplay = true;

				CItemInfo* lpItemInfo = new CItemInfo();
				 //add items text
				int pos = (*list_Iter)->FilePath.ReverseFind('\\');
				lpItemInfo->SetItemText((*list_Iter)->FilePath.Right((*list_Iter)->FilePath.GetLength()-pos-1));
				//add subitem text
				lpItemInfo->SetSubItemText(0, (*list_Iter)->FilePath);
				lpItemInfo->SetSubItemText(1, FileSize2String((*list_Iter)->FileSize)); //size

				lpItemInfo->SetSubItemText(2, CTime2String((*list_Iter)->CreateTime)); //CreateTime
				lpItemInfo->SetSubItemText(3, CTime2String((*list_Iter)->AccessTime)); //AccessTime
				lpItemInfo->SetSubItemText(4, CTime2String((*list_Iter)->WriteTime)); //WriteTime

				lpItemInfo->SetSubItemText(5, (*list_Iter)->IsSystem? Yes:No);
				lpItemInfo->SetSubItemText(6, (*list_Iter)->IsReadOnly? Yes:No);

				//insert the iteminfo with ParentPtr
				CTreeItem* pParent = InsertItem(pRoot, lpItemInfo);
			}
		}
	}

	//could now expand one level
	Expand(GetRootItem(0), 0);   //listview index 0
	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	SetItemState(0, uflag, uflag);
}

void CTempCleanerGrid::DeleteSelect()
{
	unsigned char delete_readonly = 0;
	int nItems = GetRootCount();
	if (nItems > 0)
	{
		POSITION pRootPos = GetRootHeadPosition();
		while(pRootPos != NULL)
		{
			CTreeItem *pRoot =(CTreeItem*)GetNextRoot(pRootPos);
			if(pRoot!=NULL)
			{
				POSITION pos = pRoot->m_listChild.GetHeadPosition();
				for (int i=0; pos; i++)
				{
					CTreeItem* pChildItem = (CTreeItem*)pRoot->m_listChild.GetAt(pos);
					pRoot->m_listChild.GetNext(pos);

					CItemInfo *pInfo = GetData(pChildItem);
					if(pInfo->GetCheck())
					{
						CString path = pInfo->GetSubItem(0);
						BOOL Res = ::DeleteFile(path);
						//只读文件
						if(!Res&&((GetFileAttributes(path)&FILE_ATTRIBUTE_READONLY)==FILE_ATTRIBUTE_READONLY))
						{
							bool operation=false;
							if(delete_readonly==0)//0 提示对话框
							{
								CDeleteReadOnlyNoticeDlg dlg;

								dlg.m_notice_text.Format("\"%s\"%s", path, theApp.LoadString(IDS_DeleteReadOnlyFileNotice0));
								if(dlg.DoModal()==IDOK)
								{
									operation=true;
									if(dlg.m_same_operation)
										delete_readonly=1;
								}
							}
							else if(delete_readonly==1)//1执行相同操作
								operation=true;

							if(operation)
							{
								SetFileAttributes(path,0);//0一般，1 只读，2 隐藏，4系统
								Res = ::DeleteFile(path);
							}
						}
						if(Res)
							DeleteItemEx(pChildItem, GetCurIndex(pChildItem));
					}
				}
			}
		}		
	}//nItems
}

//this is my override of GetIcon, override this to set what ever icon suits you
int CTempCleanerGrid::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int n = GetData(pItem)->GetImage();
		if(n!=-1)
			return n;
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;
}


//override called when OnLButtondown
void CTempCleanerGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	//now I am sure I added a combobox some where, so check for this control
	/*CTreeItem* pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType))
		{	
			if(ctrlType==CItemInfo::combobox) 
			{
					CStringList* list=NULL;
					pInfo->GetListData(ht.iSubItem-1, list);
					CComboBox * pList = ShowList(ht.iItem, ht.iSubItem, list);
			}
		}								
		else //activate default edit control
			CSuperGridCtrl::OnControlLButtonDown(nFlags, point, ht);
		
	}*/
}

//展开完成
BOOL CTempCleanerGrid::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}

int CTempCleanerGrid::CompareFileSizeString(const void* p1, const void* p2)
{
	CTreeItem * pChild1 = *(CTreeItem**)p1;
	CTreeItem * pChild2 = *((CTreeItem**)p2);
	CItemInfo *pItem1=(*pChild1).m_lpNodeInfo;
	CItemInfo *pItem2=(*pChild2).m_lpNodeInfo;

	int cm=0;
	if(m_SortSubItem==0)
		cm = String2FileSize(pItem1->GetItemText())-String2FileSize(pItem2->GetItemText());
	else
		cm = String2FileSize(pItem1->GetSubItem(m_SortSubItem-1))-String2FileSize(pItem2->GetSubItem(m_SortSubItem-1));
	return m_SortSubAscending? cm:-cm;
}

void CTempCleanerGrid::SortItem(CTreeItem** ppSortArray, int nChildren, int SubItem, BOOL bAscending)
{
	if(SubItem==2) //File Size
	{
		m_SortSubItem = SubItem;
		m_SortSubAscending = bAscending;

		qsort(ppSortArray, nChildren, sizeof(CTreeItem*), CompareFileSizeString);
	}
	else
		CSuperGridCtrl::SortItem(ppSortArray, nChildren, SubItem, bAscending);
}

void CTempCleanerGrid::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	CSuperGridCtrl::OnDblclk(pNMHDR, pResult);

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int row = pNMListView->iItem;
	int col = pNMListView->iSubItem;

	if(row!=-1)
	{
		CTreeItem *pSelItem = GetTreeItem(row);
		if (pSelItem != NULL)
		{
			if(!IsRoot(pSelItem))
			{
				CItemInfo *pItem = GetData(pSelItem);		
				CString path = pItem->GetSubItem(0);
				OpenDirectorySelectFile(this->m_hWnd, path);
			}
		}
	}
}
