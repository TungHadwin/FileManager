// DeleteReadOnlyNoticeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "DeleteReadOnlyNoticeDlg.h"
#include "afxdialogex.h"


// CDeleteReadOnlyNoticeDlg dialog

IMPLEMENT_DYNAMIC(CDeleteReadOnlyNoticeDlg, CDialog)

CDeleteReadOnlyNoticeDlg::CDeleteReadOnlyNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteReadOnlyNoticeDlg::IDD, pParent)
	, m_same_operation(FALSE)
	, m_notice_text(_T(""))
{

}

CDeleteReadOnlyNoticeDlg::~CDeleteReadOnlyNoticeDlg()
{
}

void CDeleteReadOnlyNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_same_operation);
	DDX_Text(pDX, IDC_NOTICE_TEXT, m_notice_text);
}


BEGIN_MESSAGE_MAP(CDeleteReadOnlyNoticeDlg, CDialog)
END_MESSAGE_MAP()


// CDeleteReadOnlyNoticeDlg message handlers
