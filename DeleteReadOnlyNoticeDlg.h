#pragma once


// CDeleteReadOnlyNoticeDlg dialog

class CDeleteReadOnlyNoticeDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeleteReadOnlyNoticeDlg)

public:
	CDeleteReadOnlyNoticeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeleteReadOnlyNoticeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_DELETE_FILE_NOTICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_same_operation;
	CString m_notice_text;
};
