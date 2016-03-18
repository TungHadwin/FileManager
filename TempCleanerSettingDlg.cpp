// TempCleanerSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "TempCleanerSettingDlg.h"
#include "afxdialogex.h"


// TempCleanerSettingDlg dialog

IMPLEMENT_DYNAMIC(TempCleanerSettingDlg, CDialog)

TempCleanerSettingDlg::TempCleanerSettingDlg(std::list<TempCleanerInfo*>& cleaner_list, CWnd* pParent /*=NULL*/)
	: CDialog(TempCleanerSettingDlg::IDD, pParent)
	, m_cleaner_list(cleaner_list)
	, m_ext_string(_T("tmp"))
	, m_folder_string(_T(""))
	, select_item_index(-1)
{
	m_list.SetToolTipInfo(0, theApp.LoadString(IDS_ListCtrl_ToolTip_DesString));
}

TempCleanerSettingDlg::~TempCleanerSettingDlg()
{
}

void TempCleanerSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHECK_INFO, m_list);
	DDX_Control(pDX, IDC_FOLDER_EDITBROWSE, m_folder_ctrl);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btn_add);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btn_delete);
	DDX_Text(pDX, IDC_EDIT1, m_ext_string);
	DDX_Text(pDX, IDC_FOLDER_EDITBROWSE, m_folder_string);
}


BEGIN_MESSAGE_MAP(TempCleanerSettingDlg, CDialog)
	ON_EN_UPDATE(IDC_FOLDER_EDITBROWSE, &TempCleanerSettingDlg::OnEnUpdateFolderEditbrowse)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &TempCleanerSettingDlg::OnBnClickedButtonAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHECK_INFO, &TempCleanerSettingDlg::OnNMDblclkListCheckInfo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CHECK_INFO, &TempCleanerSettingDlg::OnHdnItemchangedListCheckInfo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &TempCleanerSettingDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// TempCleanerSettingDlg message handlers


BOOL TempCleanerSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SHOWSELALWAYS | LVS_EX_CHECKBOXES);
	//插入列
	m_list.InsertColumn(0, theApp.LoadString(IDS_FullPath), LVCFMT_LEFT, 380);        
	m_list.InsertColumn(1, theApp.LoadString(IDS_FileExt), LVCFMT_LEFT, 60); 
	int index=0;
	for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter,++index) 
	{
		m_list.InsertItem(index, (*list_Iter)->m_folder_path);
		m_list.SetItemText(index, 1, (*list_Iter)->m_ext);
		m_list.SetCheck(index, (*list_Iter)->m_check);
	}

	m_folder_ctrl.EnableFolderBrowseButton();

	m_btn_add.EnableWindow(FALSE);
	m_btn_delete.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void TempCleanerSettingDlg::OnEnUpdateFolderEditbrowse()
{
	UpdateData(TRUE);
	m_btn_add.EnableWindow(!m_folder_string.IsEmpty());
}


void TempCleanerSettingDlg::OnBnClickedButtonAdd()
{
	UpdateData(TRUE);

	bool haved = false;
	for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
	{
		if( ((*list_Iter)->m_folder_path==m_folder_string) && ((*list_Iter)->m_ext==m_ext_string) )
		{
			haved = true;
			break;
		}
	}

	if(!haved)
	{
		int size = m_cleaner_list.size();
		m_list.InsertItem(size, m_folder_string);
		m_list.SetItemText(size, 1, m_ext_string);
		m_list.SetCheck(size, true);

		TempCleanerInfo* info = new TempCleanerInfo(m_folder_string, m_ext_string, true);
		m_cleaner_list.push_back(info);
	}
}


void TempCleanerSettingDlg::OnNMDblclkListCheckInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int row = pNMListView->iItem;
	int col = pNMListView->iSubItem;

	CString path = m_list.GetItemText(row,0);
	OpenDirectory(this->m_hWnd, path);

	*pResult = 0;
}


void TempCleanerSettingDlg::OnHdnItemchangedListCheckInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->uChanged==LVIF_STATE)
	{
		
		//CheckBox
		if((pNMListView->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
			&& (pNMListView->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
			) 
		{ 
			//TRACE("Item %d is checked\n", pNMListView->iItem);
			for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
			{
				if( ((*list_Iter)->m_folder_path==m_list.GetItemText(pNMListView->iItem, 0)) && ((*list_Iter)->m_ext==m_list.GetItemText(pNMListView->iItem, 1)) )
				{
					(*list_Iter)->m_check = true;
					break;
				}
			}
		} 
		else if((pNMListView->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
					&& (pNMListView->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
					) 
		{ 
			//TRACE("Item %d is unchecked\n", pNMListView->iItem); 
			for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
			{
				if( ((*list_Iter)->m_folder_path==m_list.GetItemText(pNMListView->iItem, 0)) && ((*list_Iter)->m_ext==m_list.GetItemText(pNMListView->iItem, 1)) )
				{
					(*list_Iter)->m_check = false;
					break;
				}
			}
		}

		if(pNMListView->uNewState & LVIS_SELECTED)
		{
			//处理内容
			select_item_index = pNMListView->iItem;

			m_btn_delete.EnableWindow(TRUE);
		}
	}
	*pResult = 0;
}


void TempCleanerSettingDlg::OnBnClickedButtonDelete()
{
	if(select_item_index!=-1)
	{
		CString folder = m_list.GetItemText(select_item_index, 0);
		CString ext = m_list.GetItemText(select_item_index, 1);
		for (auto list_Iter = m_cleaner_list.begin(); list_Iter != m_cleaner_list.end(); ++list_Iter) 
		{
			if( ((*list_Iter)->m_folder_path==folder) && ((*list_Iter)->m_ext==ext) )
			{
				delete (*list_Iter);
				m_cleaner_list.erase(list_Iter);
				break;
			}
		}
		m_list.DeleteItem(select_item_index);

		select_item_index = -1;
		m_btn_delete.EnableWindow(FALSE);
	}
}
