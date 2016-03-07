#pragma once
#include "afxcmn.h"
#include "fileprocess.h"
#include <list>
#include "ShowProcessDlg.h"
#include "ToolTipListCtrl.h"

// CEmptyFileView form view

class CEmptyFileView : public CFormView
{
	DECLARE_DYNCREATE(CEmptyFileView)

public:
	CEmptyFileView();           // protected constructor used by dynamic creation
	virtual ~CEmptyFileView();

	static BOOL WINAPI EmptyFolderEnumerateFunc (CString& FolderPath, bool IsSystem, bool IsReadOnly, void* pUserData); 
	static BOOL WINAPI ExportFoldersToFileFunc (CString& FolderPath, void* pUserData); 

public:
	enum { IDD = IDD_EMPTYFILEVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnSelectFolder();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedCheckSystem();
	afx_msg void OnBnClickedCheckReadonly();
	
	afx_msg void OnBnClickedBtnDeleteSelect();
private:
	CToolTipListCtrl m_empty_files_list;

	CString Yes, No;

	std::list<EmptyFolderInfo*> m_empty_file_list;

	CString folderpath;

	BOOL m_check_all;
	BOOL m_check_system;
	BOOL m_check_readonly;

	CShowProcessDlg m_show_dlg;

	void ListFolders();
	void AdjustListColumnWidth();
	void DestoryAll();
	BOOL TEmptyFolderEnumerateFunc(CString& FolderPath, bool IsSystem, bool IsReadOnly);
	BOOL TExportFoldersToFileFunc (CString& FolderPath); 
public:
	afx_msg void OnNMDblclkListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnExportToFile();
	afx_msg void OnNMRClickListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult);
};


