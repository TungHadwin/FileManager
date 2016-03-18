// EmptyFileView.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "EmptyFileView.h"
#include "DeleteReadOnlyNoticeDlg.h"
// CEmptyFileView

IMPLEMENT_DYNCREATE(CEmptyFileView, CFormView)

CEmptyFileView::CEmptyFileView()
	: CFormView(CEmptyFileView::IDD)
	, folderpath("")
	, m_check_all(FALSE)
	, m_check_system(FALSE)
	, m_check_readonly(FALSE)
{
	Yes = theApp.LoadString(IDS_Yes);
	No = theApp.LoadString(IDS_No);

	m_empty_files_list.SetToolTipInfo(1, theApp.LoadString(IDS_ListCtrl_ToolTip_DesString));
}

CEmptyFileView::~CEmptyFileView()
{
	DestoryAll();
}

void CEmptyFileView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMPTY_FILES, m_empty_files_list);
	DDX_Check(pDX, IDC_CHECK_ALL, m_check_all);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_check_system);
	DDX_Check(pDX, IDC_CHECK_READONLY, m_check_readonly);
}

BEGIN_MESSAGE_MAP(CEmptyFileView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SELECT_FOLDER, &CEmptyFileView::OnBnClickedBtnSelectFolder)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CEmptyFileView::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM, &CEmptyFileView::OnBnClickedCheckSystem)
	ON_BN_CLICKED(IDC_CHECK_READONLY, &CEmptyFileView::OnBnClickedCheckReadonly)
	ON_BN_CLICKED(IDC_BTN_DELETE_SELECT, &CEmptyFileView::OnBnClickedBtnDeleteSelect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EMPTY_FILES, &CEmptyFileView::OnNMDblclkListEmptyFiles)
	ON_COMMAND(ID_EXPORT_TO_FILE, &CEmptyFileView::OnExportToFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_EMPTY_FILES, &CEmptyFileView::OnNMRClickListEmptyFiles)
END_MESSAGE_MAP()


// CEmptyFileView diagnostics

#ifdef _DEBUG
void CEmptyFileView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEmptyFileView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEmptyFileView message handlers


void CEmptyFileView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_show_dlg.Create(CShowProcessDlg::IDD,this); //创建一个非模态对话框);

	m_empty_files_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SHOWSELALWAYS| LVS_EX_CHECKBOXES);
	m_empty_files_list.InsertColumn(0, theApp.LoadString(IDS_FileName), LVCFMT_LEFT, 128);         //插入列
	m_empty_files_list.InsertColumn(1, theApp.LoadString(IDS_FullPath), LVCFMT_LEFT, 138); 
	m_empty_files_list.InsertColumn(2, theApp.LoadString(IDS_FileSize), LVCFMT_RIGHT, 80); 
	m_empty_files_list.InsertColumn(3, theApp.LoadString(IDS_FileExt), LVCFMT_RIGHT, 80); 
	m_empty_files_list.InsertColumn(4, theApp.LoadString(IDS_CreateTime), LVCFMT_RIGHT, 80); 
	m_empty_files_list.InsertColumn(5, theApp.LoadString(IDS_AccessTime), LVCFMT_RIGHT, 80); 
	m_empty_files_list.InsertColumn(6, theApp.LoadString(IDS_WriteTime), LVCFMT_RIGHT, 80); 
	m_empty_files_list.InsertColumn(7, theApp.LoadString(IDS_SystemFile), LVCFMT_RIGHT, 80); 
	m_empty_files_list.InsertColumn(8, theApp.LoadString(IDS_ReadOnly), LVCFMT_RIGHT, 80); 

	AdjustListColumnWidth();
}


void CEmptyFileView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	if(bActivate)
	{
		CDocument* pDoc = GetDocument();
		pDoc->SetTitle(theApp.LoadString(IDS_EmptyFileManager));
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CEmptyFileView::OnSize(UINT nType, int cx, int cy)
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

void CEmptyFileView::AdjustListColumnWidth()
{
	if(m_empty_files_list.GetSafeHwnd())
	{
		CRect rect;
		m_empty_files_list.GetWindowRect(rect);
		ScreenToClient(rect);
		m_empty_files_list.SetColumnWidth(0,(rect.Width()-60*2-120*3-80*2)/3);
		m_empty_files_list.SetColumnWidth(1,(rect.Width()-60*2-120*3-80*2)*2/3-4-8);
		m_empty_files_list.SetColumnWidth(2,60);//size
		m_empty_files_list.SetColumnWidth(3,60);//ext
		m_empty_files_list.SetColumnWidth(4,120);
		m_empty_files_list.SetColumnWidth(5,120);
		m_empty_files_list.SetColumnWidth(6,120);
		m_empty_files_list.SetColumnWidth(7,80);
		m_empty_files_list.SetColumnWidth(8,80);
	}
}

BOOL WINAPI CEmptyFileView::EmptyFileEnumerateFunc (CString& FolderPath, unsigned int FileSize, CTime& CreateTime, CTime& AccessTime, CTime& WriteTime, bool IsSystem, bool IsReadOnly, void* pUserData)
{
	return ((CEmptyFileView*)pUserData)->TEmptyFileEnumerateFunc(FolderPath, FileSize, CreateTime, AccessTime, WriteTime, IsSystem, IsReadOnly);
}

BOOL CEmptyFileView::TEmptyFileEnumerateFunc(CString& FolderPath, unsigned int FileSize, CTime& CreateTime, CTime& AccessTime, CTime& WriteTime, bool IsSystem, bool IsReadOnly)
{
	EmptyFileInfo* info = new EmptyFileInfo(FolderPath, FileSize, CreateTime, AccessTime, WriteTime, IsSystem, IsReadOnly);
	m_empty_file_list.push_back(info);

	m_show_dlg.SetDispText(FolderPath);
	return true;
}

void CEmptyFileView::OnBnClickedBtnSelectFolder()
{
	folderpath = ShowFolderBrowse(m_hWnd);
	if(!folderpath.IsEmpty())
	{
		
		m_show_dlg.ShowWindow(SW_SHOWNORMAL); //显示非模态对话框

		DestoryAll();
		//doEmptyFolderEnumeration(folderpath, m_empty_file_list);
		doEmptyFileEnumeration(folderpath, EmptyFileEnumerateFunc, this);

		m_show_dlg.ShowWindow(SW_HIDE); //显示非模态对话框

		ListFolders();
	}
}

void CEmptyFileView::ListFolders()
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

		CString str;
		int pos = (*list_Iter)->FilePath.ReverseFind('\\');
		m_empty_files_list.InsertItem(empty_files_add_index, (*list_Iter)->FilePath.Right((*list_Iter)->FilePath.GetLength()-pos-1));      
		m_empty_files_list.SetItemText(empty_files_add_index, 1, (*list_Iter)->FilePath);

		str.Format("%dB",(*list_Iter)->FileSize);
		m_empty_files_list.SetItemText(empty_files_add_index, 2, str); //size

		pos = (*list_Iter)->FilePath.ReverseFind('.');
		m_empty_files_list.SetItemText(empty_files_add_index, 3, (*list_Iter)->FilePath.Right((*list_Iter)->FilePath.GetLength()-pos-1)); //ext

		m_empty_files_list.SetItemText(empty_files_add_index, 4, CTime2String((*list_Iter)->CreateTime)); //CreateTime
		m_empty_files_list.SetItemText(empty_files_add_index, 5, CTime2String((*list_Iter)->AccessTime)); //AccessTime
		m_empty_files_list.SetItemText(empty_files_add_index, 6, CTime2String((*list_Iter)->WriteTime)); //WriteTime

		m_empty_files_list.SetItemText(empty_files_add_index, 7, (*list_Iter)->IsSystem? Yes:No);
		m_empty_files_list.SetItemText(empty_files_add_index, 8, (*list_Iter)->IsReadOnly? Yes:No);
		m_empty_files_list.SetItemData(empty_files_add_index, (DWORD)empty_files_add_index);    // 一般就设置一个唯一的值.不唯一排序可能有点乱,
		empty_files_add_index++;
	}
}

void CEmptyFileView::DestoryAll()
{
	for (auto list_Iter = m_empty_file_list.begin(); list_Iter != m_empty_file_list.end(); ++list_Iter) 
	{
			delete (*list_Iter);
			(*list_Iter) = NULL;
	}
	m_empty_file_list.erase(m_empty_file_list.begin(), m_empty_file_list.end());
	m_empty_file_list.clear();
}


void CEmptyFileView::OnBnClickedCheckAll()
{
	UpdateData(TRUE);
	//ListFolders();
	for(int i=0; i<m_empty_files_list.GetItemCount(); i++)
		m_empty_files_list.SetCheck(i, m_check_all);
}


void CEmptyFileView::OnBnClickedCheckSystem()
{
	UpdateData(TRUE);
	ListFolders();
}


void CEmptyFileView::OnBnClickedCheckReadonly()
{
	UpdateData(TRUE);
	ListFolders();
}


void CEmptyFileView::OnBnClickedBtnDeleteSelect()
{
	unsigned char delete_readonly = 0;
	for(int i=0; i<m_empty_files_list.GetItemCount();)
	{
		if(m_empty_files_list.GetCheck(i))
		{
			CString path = m_empty_files_list.GetItemText(i,1);

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
				m_empty_files_list.DeleteItem(i);
			else
				i++;
		}
		else
			i++;
	}
}


void CEmptyFileView::OnNMDblclkListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int row = pNMListView->iItem;
	int col = pNMListView->iSubItem;

	if(row!=-1)
	{
		CString path = m_empty_files_list.GetItemText(row,1);
		OpenDirectorySelectFile(this->m_hWnd, path);
	}
	*pResult = 0;
}

//----------------------------------------------------------------------------------------------
void CEmptyFileView::OnNMRClickListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult)
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


BOOL WINAPI CEmptyFileView::ExportFoldersToFileFunc(CString& FolderPath, void* pUserData)
{
	return ((CEmptyFileView*)pUserData)->TExportFoldersToFileFunc(FolderPath);
}

BOOL CEmptyFileView::TExportFoldersToFileFunc (CString& FolderPath)
{
	m_show_dlg.SetDispText(FolderPath);
	return true;
}

void CEmptyFileView::OnExportToFile()
{
	CFileDialog dlg(FALSE, "", "", NULL, "Excel Files (*.csv)|*.csv|", this);
	if(dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();

		m_show_dlg.ShowWindow(SW_SHOWNORMAL); //显示非模态对话框

		ExportFilesToFile(pathname, 
							theApp.LoadString(IDS_FileName),  theApp.LoadString(IDS_FullPath), theApp.LoadString(IDS_FileSize), theApp.LoadString(IDS_FileExt), 
								theApp.LoadString(IDS_CreateTime), theApp.LoadString(IDS_AccessTime), theApp.LoadString(IDS_WriteTime), theApp.LoadString(IDS_SystemFile), theApp.LoadString(IDS_ReadOnly), Yes, No,
							m_empty_file_list, ExportFoldersToFileFunc, this);

		m_show_dlg.ShowWindow(SW_HIDE); //显示非模态对话框
	}
}


