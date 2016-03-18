#pragma once
#include "afxcmn.h"
#include "fileprocess.h"
#include <list>
#include "ShowProcessDlg.h"
#include "TempCleanerGrid.h"

// CEmptyFileView form view

class CTempCleanerView : public CFormView
{
	DECLARE_DYNCREATE(CTempCleanerView)

public:
	CTempCleanerView();           // protected constructor used by dynamic creation
	virtual ~CTempCleanerView();

	static BOOL WINAPI TempFileEnumerateFunc (CString& FolderPath, void* pUserData); 
	static BOOL WINAPI ExportFoldersToFileFunc (CString& FolderPath, void* pUserData); 

public:
	enum { IDD = IDD_TEMPCLEANER_VIEW };
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
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedCheckSystem();
	afx_msg void OnBnClickedCheckReadonly();	
	afx_msg void OnBnClickedBtnDeleteSelect();
	afx_msg void OnExportToFile();
	afx_msg void OnNMRClickListEmptyFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnScan();
	afx_msg void OnBnClickedBtnSetting();
private:
	CTempCleanerGrid m_list;

	CString Yes, No;

	std::list<TempCleanerInfo*> m_cleaner_list;

	CString folderpath;

	BOOL m_check_all;
	BOOL m_check_system;
	BOOL m_check_readonly;

	CShowProcessDlg m_show_dlg;

	void ListFolders();
	void AdjustListColumnWidth();
	void DestoryAll();
	BOOL TTempFileEnumerateFunc(CString& FolderPath);
	BOOL TExportFoldersToFileFunc (CString& FolderPath); 

	void LoadRegedits();
	void SaveRegedits();
};


