// EmptyFileView.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "EmptyFolderView.h"
// CEmptyFolderView

IMPLEMENT_DYNCREATE(CEmptyFolderView, CFormView)

CEmptyFolderView::CEmptyFolderView()
	: CFormView(CEmptyFolderView::IDD)
	, folderpath("")
	, m_check_all(FALSE)
	, m_check_system(FALSE)
	, m_check_readonly(FALSE)
{
	Yes = theApp.LoadString(IDS_Yes);
	No = theApp.LoadString(IDS_No);

	m_empty_files_list.SetToolTipInfo(1, theApp.LoadString(IDS_ListCtrl_ToolTip_DesString));
}

CEmptyFolderView::~CEmptyFolderView()
{
	DestoryAll();
}

void CEmptyFolderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMPTY_FILES, m_empty_files_list);
	DDX_Check(pDX, IDC_CHECK_ALL, m_check_all);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_check_system);
	DDX_Check(pDX, IDC_CHECK_READONLY, m_check_readonly);
}

BEGIN_MESSAGE_MAP(CEmptyFolderView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SELECT_FOLDER, &CEmptyFolderView::OnBnClickedBtnSelectFolder)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CEmptyFolderView::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM, &CEmptyFolderView::OnBnClickedCheckSystem)
	ON_BN_CLICKED(IDC_CHECK_READONLY, &CEmptyFolderView::OnBnClickedCheckReadonly)
	ON_BN_CLICKED(IDC_BTN_DELETE_SELECT, &CEmptyFolderView::OnBnClickedBtnDeleteSelect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EMPTY_FILES, &CEmptyFolderView::OnNMDblclkListEmptyFiles)
	ON_COMMAND(ID_EXPORT_TO_FILE, &CEmptyFolderView::OnExportToFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_EMPTY_FILES, &CEmptyFolderView::OnNMRClickListEmptyFiles)
END_MESSAGE_MAP()


// CEmptyFolderView diagnostics

#ifdef _DEBUG
void CEmptyFolderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEmptyFolderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEmptyFolderView message handlers


void CEmptyFolderView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_show_dlg.Create(CShowProcessDlg::IDD,this); //创建一个非模态对话框);

	m_empty_files_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SHOWSELALWAYS| LVS_EX_CHECKBOXES);
	m_empty_files_list.InsertColumn(0, theApp.LoadString(IDS_FolderName), LVCFMT_LEFT, 128);         //插入列
	m_empty_files_list.InsertColumn(1, theApp.LoadString(IDS_FullPath), LVCFMT_LEFT, 138); 
	m_empty_files_list.InsertColumn(2, theApp.LoadString(IDS_SystemFile), LVCFMT_LEFT, 80); 
	m_empty_files_list.InsertColumn(3, theApp.LoadString(IDS_ReadOnly), LVCFMT_LEFT, 80); 

	AdjustListColumnWidth();
}


void CEmptyFolderView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	if(bActivate)
	{
		CDocument* pDoc = GetDocument();
		pDoc->SetTitle(theApp.LoadString(IDS_EmptyFolderManager));
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CEmptyFolderView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if((cx==0)&&(cy==0))
		return;
	SetScrollSizes(MM_TEXT,CSize(cx,cy));

	CRect client,ctrlrect;
	GetClientRect(client);

#define ENGE_WIDTH 10
	CWnd* pwnd=GetDlgItem(IDC_BTN_SELECT_FOLDER);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(ctrlrect);
		ScreenToClient(ctrlrect);
		ctrlrect.left=client.right-ENGE_WIDTH-ctrlrect.Width();
		ctrlrect.right=client.right-ENGE_WIDTH;
		pwnd->MoveWindow(ctrlrect);
	}

	pwnd=GetDlgItem(IDC_CHECK_ALL);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(ctrlrect);
		ScreenToClient(ctrlrect);
		ctrlrect.left=client.right-ENGE_WIDTH-ctrlrect.Width();
		ctrlrect.right=client.right-ENGE_WIDTH;
		pwnd->MoveWindow(ctrlrect);
	}

	pwnd=GetDlgItem(IDC_CHECK_SYSTEM);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(ctrlrect);
		ScreenToClient(ctrlrect);
		ctrlrect.left=client.right-ENGE_WIDTH-ctrlrect.Width();
		ctrlrect.right=client.right-ENGE_WIDTH;
		pwnd->MoveWindow(ctrlrect);
	}

	pwnd=GetDlgItem(IDC_CHECK_READONLY);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(ctrlrect);
		ScreenToClient(ctrlrect);
		ctrlrect.left=client.right-ENGE_WIDTH-ctrlrect.Width();
		ctrlrect.right=client.right-ENGE_WIDTH;
		pwnd->MoveWindow(ctrlrect);
	}

	pwnd=GetDlgItem(IDC_BTN_DELETE_SELECT);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(ctrlrect);
		ScreenToClient(ctrlrect);
		ctrlrect.left=client.right-ENGE_WIDTH-ctrlrect.Width();
		ctrlrect.right=client.right-ENGE_WIDTH;
		pwnd->MoveWindow(ctrlrect);
	}

	CRect rect;
	pwnd=GetDlgItem(IDC_LIST_EMPTY_FILES);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(rect);
		ScreenToClient(rect);
		rect.right=ctrlrect.left-ENGE_WIDTH;
		rect.bottom=client.bottom;
		pwnd->MoveWindow(rect);
	}

	AdjustListColumnWidth();
}

void CEmptyFolderView::AdjustListColumnWidth()
{
	if(m_empty_files_list.GetSafeHwnd())
	{
		CRect rect;
		m_empty_files_list.GetWindowRect(rect);
		ScreenToClient(rect);
		m_empty_files_list.SetColumnWidth(0,rect.Width()/3);
		m_empty_files_list.SetColumnWidth(1,rect.Width()*2/3-4-168);
		m_empty_files_list.SetColumnWidth(2,80);
		m_empty_files_list.SetColumnWidth(3,80);
	}
}

BOOL WINAPI CEmptyFolderView::EmptyFolderEnumerateFunc (CString& FolderPath, bool IsSystem, bool IsReadOnly, void* pUserData)
{
	return ((CEmptyFolderView*)pUserData)->TEmptyFolderEnumerateFunc(FolderPath, IsSystem, IsReadOnly);
}

BOOL CEmptyFolderView::TEmptyFolderEnumerateFunc(CString& FolderPath, bool IsSystem, bool IsReadOnly)
{
	EmptyFolderInfo* info = new EmptyFolderInfo(FolderPath, IsSystem, IsReadOnly);
	m_empty_file_list.push_back(info);

	m_show_dlg.SetDispText(FolderPath);
	return true;
}

void CEmptyFolderView::OnBnClickedBtnSelectFolder()
{
	folderpath = ShowFolderBrowse(m_hWnd);
	if(!folderpath.IsEmpty())
	{
		
		m_show_dlg.ShowWindow(SW_SHOWNORMAL); //显示非模态对话框

		DestoryAll();
		//doEmptyFolderEnumeration(folderpath, m_empty_file_list);
		doEmptyFolderEnumeration(folderpath, EmptyFolderEnumerateFunc, this);

		m_show_dlg.ShowWindow(SW_HIDE); //显示非模态对话框

		ListFolders();
	}
}

void CEmptyFolderView::ListFolders()
{
	m_empty_files_list.DeleteAllItems();

	int empty_files_add_index=0;
	for (auto list_Iter = m_empty_file_list.begin(); list_Iter != m_empty_file_list.end(); ++list_Iter) 
	{
		if( ((*list_Iter)->IsSystem&&(!m_check_system)) || ((*list_Iter)->IsReadOnly&&(!m_check_readonly)) )
		{
			(*list_Iter)->IsDisplay = false;
			continue;
		}

		(*list_Iter)->IsDisplay = true;
		int pos = (*list_Iter)->FolderPath.ReverseFind('\\');
		m_empty_files_list.InsertItem(empty_files_add_index, (*list_Iter)->FolderPath.Right((*list_Iter)->FolderPath.GetLength()-pos-1));      
		m_empty_files_list.SetItemText(empty_files_add_index, 1, (*list_Iter)->FolderPath);
		m_empty_files_list.SetItemText(empty_files_add_index, 2, (*list_Iter)->IsSystem? Yes:No);
		m_empty_files_list.SetItemText(empty_files_add_index, 3, (*list_Iter)->IsReadOnly? Yes:No);
		m_empty_files_list.SetItemData(empty_files_add_index, (DWORD)empty_files_add_index);    // 一般就设置一个唯一的值.不唯一排序可能有点乱,
		empty_files_add_index++;
	}


	/*//先处理非系统
	for (auto list_Iter = m_empty_file_list.begin(); list_Iter != m_empty_file_list.end(); ++list_Iter) 
	{
		if( ((*list_Iter)->IsSystem) || ((*list_Iter)->IsReadOnly&&(!m_check_readonly)) )
		{
			(*list_Iter)->IsDisplay = false;
			continue;
		}

		(*list_Iter)->IsDisplay = true;
		int pos = (*list_Iter)->FolderPath.ReverseFind('\\');
		m_empty_files_list.InsertItem(empty_files_add_index, (*list_Iter)->FolderPath.Right((*list_Iter)->FolderPath.GetLength()-pos-1));      
		m_empty_files_list.SetItemText(empty_files_add_index, 1, (*list_Iter)->FolderPath);
		m_empty_files_list.SetItemText(empty_files_add_index, 2, (*list_Iter)->IsSystem? Yes:No);
		m_empty_files_list.SetItemText(empty_files_add_index, 3, (*list_Iter)->IsReadOnly? Yes:No);
		m_empty_files_list.SetItemData(empty_files_add_index, (DWORD)empty_files_add_index);    // 一般就设置一个唯一的值.不唯一排序可能有点乱,
		empty_files_add_index++;
	}
	//处理系统
	for (auto list_Iter = m_empty_file_list.begin(); list_Iter != m_empty_file_list.end(); ++list_Iter) 
	{
		if((*list_Iter)->IsSystem&&(m_check_system))
		{
			if((*list_Iter)->IsReadOnly&&(!m_check_readonly))
				continue;

			(*list_Iter)->IsDisplay = true;
			int pos = (*list_Iter)->FolderPath.ReverseFind('\\');
			m_empty_files_list.InsertItem(empty_files_add_index, (*list_Iter)->FolderPath.Right((*list_Iter)->FolderPath.GetLength()-pos-1));      
			m_empty_files_list.SetItemText(empty_files_add_index, 1, (*list_Iter)->FolderPath);
			m_empty_files_list.SetItemText(empty_files_add_index, 2, (*list_Iter)->IsSystem? Yes:No);
			m_empty_files_list.SetItemText(empty_files_add_index, 3, (*list_Iter)->IsReadOnly? Yes:No);
			m_empty_files_list.SetItemData(empty_files_add_index, (DWORD)empty_files_add_index);    // 一般就设置一个唯一的值.不唯一排序可能有点乱,
			empty_files_add_index++;
		}
	}*/
}

void CEmptyFolderView::DestoryAll()
{
	for (auto list_Iter = m_empty_file_list.begin(); list_Iter != m_empty_file_list.end(); ++list_Iter) 
	{
			delete (*list_Iter);
			(*list_Iter) = NULL;
	}
	m_empty_file_list.erase(m_empty_file_list.begin(), m_empty_file_list.end());
	m_empty_file_list.clear();
}


void CEmptyFolderView::OnBnClickedCheckAll()
{
	UpdateData(TRUE);
	//ListFolders();
	for(int i=0; i<m_empty_files_list.GetItemCount(); i++)
		m_empty_files_list.SetCheck(i, m_check_all);
}


void CEmptyFolderView::OnBnClickedCheckSystem()
{
	UpdateData(TRUE);
	ListFolders();
}


void CEmptyFolderView::OnBnClickedCheckReadonly()
{
	UpdateData(TRUE);
	ListFolders();
}


void CEmptyFolderView::OnBnClickedBtnDeleteSelect()
{
	for(int i=0; i<m_empty_files_list.GetItemCount();)
	{
		if(m_empty_files_list.GetCheck(i))
		{
			CString path = m_empty_files_list.GetItemText(i,1);
			::RemoveDirectory(path);
			m_empty_files_list.DeleteItem(i);
		}
		else
			i++;
	}
}


void CEmptyFolderView::OnNMDblclkListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int row = pNMListView->iItem;
	int col = pNMListView->iSubItem;

	CString path = m_empty_files_list.GetItemText(row,1);
	OpenDirectory(this->m_hWnd, path);

	*pResult = 0;
}

//----------------------------------------------------------------------------------------------
void CEmptyFolderView::OnNMRClickListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if(pNMItemActivate->iItem != -1)
	{
		CPoint   point;  
        GetCursorPos(   &point   );  
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_EXPORT, point.x, point.y, this, TRUE);
	}
	*pResult = 0;
}


BOOL WINAPI CEmptyFolderView::ExportFoldersToFileFunc(CString& FolderPath, void* pUserData)
{
	return ((CEmptyFolderView*)pUserData)->TExportFoldersToFileFunc(FolderPath);
}

BOOL CEmptyFolderView::TExportFoldersToFileFunc (CString& FolderPath)
{
	m_show_dlg.SetDispText(FolderPath);
	return true;
}

void CEmptyFolderView::OnExportToFile()
{
	CFileDialog dlg(FALSE, "", "", NULL, "Excel Files (*.csv)|*.csv|", this);
	if(dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();

		m_show_dlg.ShowWindow(SW_SHOWNORMAL); //显示非模态对话框

		ExportFoldersToFile(pathname, theApp.LoadString(IDS_FolderName),  theApp.LoadString(IDS_FullPath), theApp.LoadString(IDS_SystemFile), theApp.LoadString(IDS_ReadOnly), Yes, No,
							m_empty_file_list, ExportFoldersToFileFunc, this);

		m_show_dlg.ShowWindow(SW_HIDE); //显示非模态对话框
	}
}


