#pragma once
#include "afxcmn.h"
#include "ToolTipListCtrl.h"
#include "afxeditbrowsectrl.h"
#include "fileprocess.h"
#include "afxwin.h"


// TempCleanerSettingDlg dialog

class TempCleanerSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(TempCleanerSettingDlg)

public:
	TempCleanerSettingDlg(std::list<TempCleanerInfo*>& cleaner_list, CWnd* pParent = NULL);   // standard constructor
	virtual ~TempCleanerSettingDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SCAN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnEnUpdateFolderEditbrowse();
	afx_msg void OnBnClickedButtonAdd();
private:
	CToolTipListCtrl m_list;
	CMFCEditBrowseCtrl m_folder_ctrl;
	CString m_ext_string;
	CString m_folder_string;
	CButton m_btn_add;
	CButton m_btn_delete;

	std::list<TempCleanerInfo*>& m_cleaner_list;

	int select_item_index;
public:
	
	afx_msg void OnNMDblclkListCheckInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchangedListCheckInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelete();
};
