#pragma once
#include "afxwin.h"


// CShowProcessDlg dialog

class CShowProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowProcessDlg)

public:
	CShowProcessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowProcessDlg();

	void SetDispText(CString& str){m_pro_file_now.SetWindowText(str);};
// Dialog Data
	enum { IDD = IDD_DLG_SHOW_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
private:
	CStatic m_pro_file_now;
};
