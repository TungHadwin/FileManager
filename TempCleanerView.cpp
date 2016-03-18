// EmptyFileView.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "TempCleanerView.h"
#include "TempCleanerSettingDlg.h"
#include <fstream>

// CTempCleanerView

IMPLEMENT_DYNCREATE(CTempCleanerView, CFormView)

CTempCleanerView::CTempCleanerView()
	: CFormView(CTempCleanerView::IDD)
	, folderpath("")
	, m_check_all(FALSE)
	, m_check_system(FALSE)
	, m_check_readonly(FALSE)
{
	Yes = theApp.LoadString(IDS_Yes);
	No = theApp.LoadString(IDS_No);
}

CTempCleanerView::~CTempCleanerView()
{
	SaveRegedits();
	DestoryAll();
}

void CTempCleanerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMPTY_FILES, m_list);
	DDX_Check(pDX, IDC_CHECK_ALL, m_check_all);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_check_system);
	DDX_Check(pDX, IDC_CHECK_READONLY, m_check_readonly);
}

BEGIN_MESSAGE_MAP(CTempCleanerView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_ALL, &CTempCleanerView::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM, &CTempCleanerView::OnBnClickedCheckSystem)
	ON_BN_CLICKED(IDC_CHECK_READONLY, &CTempCleanerView::OnBnClickedCheckReadonly)
	ON_BN_CLICKED(IDC_BTN_DELETE_SELECT, &CTempCleanerView::OnBnClickedBtnDeleteSelect)
	ON_COMMAND(ID_EXPORT_TO_FILE, &CTempCleanerView::OnExportToFile)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_EMPTY_FILES, &CTempCleanerView::OnNMRClickListEmptyFiles)
	ON_BN_CLICKED(IDC_BTN_SCAN, &CTempCleanerView::OnBnClickedBtnScan)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CTempCleanerView::OnBnClickedBtnSetting)
END_MESSAGE_MAP()


// CTempCleanerView diagnostics

#ifdef _DEBUG
void CTempCleanerView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTempCleanerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTempCleanerView message handlers


void CTempCleanerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	LoadRegedits();

	m_show_dlg.Create(CShowProcessDlg::IDD,this); //创建一个非模态对话框);

	m_list.InitializeGrid();
	AdjustListColumnWidth();
}


void CTempCleanerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	if(bActivate)
	{
		CDocument* pDoc = GetDocument();
		pDoc->SetTitle(theApp.LoadString(IDS_TempCleaner));
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CTempCleanerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if((cx==0)&&(cy==0))
		return;
	SetScrollSizes(MM_TEXT,CSize(cx,cy));

	CRect client,ctrlrect;
	GetClientRect(client);

#define ENGE_WIDTH 10
	CWnd* pwnd=GetDlgItem(IDC_BTN_SETTING);
	if(pwnd->GetSafeHwnd())
	{
		pwnd->GetWindowRect(ctrlrect);
		ScreenToClient(ctrlrect);
		ctrlrect.left=client.right-ENGE_WIDTH-ctrlrect.Width();
		ctrlrect.right=client.right-ENGE_WIDTH;
		pwnd->MoveWindow(ctrlrect);
	}
	
	pwnd=GetDlgItem(IDC_BTN_SCAN);
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

void CTempCleanerView::AdjustListColumnWidth()
{
	m_list.AdjustListColumnWidth();
}

BOOL WINAPI CTempCleanerView::TempFileEnumerateFunc (CString& FolderPath, void* pUserData)
{
	return ((CTempCleanerView*)pUserData)->TTempFileEnumerateFunc(FolderPath);
}

BOOL CTempCleanerView::TTempFileEnumerateFunc(CString& FolderPath)
{
	m_show_dlg.SetDispText(FolderPath);
	return true;
}

void CTempCleanerView::OnBnClickedBtnSetting()
{
	TempCleanerSettingDlg dlg(m_cleaner_list);
	dlg.DoModal();
}


void CTempCleanerView::OnBnClickedBtnScan()
{
	if(m_cleaner_list.size()>0)
	{
		for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
		{
			(*list_Iter)->DestoryAll();
		}

		m_show_dlg.ShowWindow(SW_SHOWNORMAL); //显示非模态对话框

		doTempFileEnumeration(m_cleaner_list, TempFileEnumerateFunc, this);

		m_show_dlg.ShowWindow(SW_HIDE); //显示非模态对话框

		ListFolders();
	}
	else
		OnBnClickedBtnSetting();
}

void CTempCleanerView::ListFolders()
{
	m_list.ListFolders(m_cleaner_list, m_check_system, m_check_readonly, Yes, No);
}

void CTempCleanerView::DestoryAll()
{
	for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
	{
			delete (*list_Iter);
			(*list_Iter) = NULL;
	}
	m_cleaner_list.erase(m_cleaner_list.begin(), m_cleaner_list.end());
	m_cleaner_list.clear();
}


void CTempCleanerView::OnBnClickedCheckAll()
{
	UpdateData(TRUE);
	m_list.SetCheckAll(m_check_all);
}


void CTempCleanerView::OnBnClickedCheckSystem()
{
	UpdateData(TRUE);
	ListFolders();
}


void CTempCleanerView::OnBnClickedCheckReadonly()
{
	UpdateData(TRUE);
	ListFolders();
}


void CTempCleanerView::OnBnClickedBtnDeleteSelect()
{
	m_list.DeleteSelect();
}

//----------------------------------------------------------------------------------------------
void CTempCleanerView::OnNMRClickListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult)
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


BOOL WINAPI CTempCleanerView::ExportFoldersToFileFunc(CString& FolderPath, void* pUserData)
{
	return ((CTempCleanerView*)pUserData)->TExportFoldersToFileFunc(FolderPath);
}

BOOL CTempCleanerView::TExportFoldersToFileFunc (CString& FolderPath)
{
	m_show_dlg.SetDispText(FolderPath);
	return true;
}

void CTempCleanerView::OnExportToFile()
{
	CFileDialog dlg(FALSE, "", "", NULL, "Excel Files (*.csv)|*.csv|", this);
	if(dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();

		m_show_dlg.ShowWindow(SW_SHOWNORMAL); //显示非模态对话框

		ExportTempCleanerToFile(pathname, 
							theApp.LoadString(IDS_FolderName),  theApp.LoadString(IDS_FullPath), theApp.LoadString(IDS_FileSize), 
								theApp.LoadString(IDS_CreateTime), theApp.LoadString(IDS_AccessTime), theApp.LoadString(IDS_WriteTime), theApp.LoadString(IDS_SystemFile), theApp.LoadString(IDS_ReadOnly), Yes, No,
							m_cleaner_list, ExportFoldersToFileFunc, this);

		m_show_dlg.ShowWindow(SW_HIDE); //显示非模态对话框
	}
}

void CTempCleanerView::LoadRegedits()
{
	std::fstream file(".\\TempCleanerData.csv", std::fstream::in);
	if(file.is_open())
	{
		char buffer[256];
		file.getline(buffer,256);
		while(!file.eof())
		{
			bool check = buffer[0]=='1'? true:false;
			char* buf = buffer+2;
			char* pos = strchr(buf,',');
			TempCleanerInfo* info = new TempCleanerInfo(CString(buf,pos-buf), CString(pos+1), check);
			m_cleaner_list.push_back(info);

			file.getline(buffer,256);
		}
	}
}

void CTempCleanerView::SaveRegedits()
{
	std::fstream file(".\\TempCleanerData.csv", std::fstream::out);
	for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
		file<< (*list_Iter)->m_check <<','<< (*list_Iter)->m_folder_path <<','<< (*list_Iter)->m_ext <<"\n";
	file.close();
	file.clear();
}

	