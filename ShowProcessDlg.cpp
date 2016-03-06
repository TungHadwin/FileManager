// ShowProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileManager.h"
#include "ShowProcessDlg.h"
#include "afxdialogex.h"


// CShowProcessDlg dialog

IMPLEMENT_DYNAMIC(CShowProcessDlg, CDialog)

CShowProcessDlg::CShowProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowProcessDlg::IDD, pParent)
{

}

CShowProcessDlg::~CShowProcessDlg()
{
}

void CShowProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NOW, m_pro_file_now);
}


BEGIN_MESSAGE_MAP(CShowProcessDlg, CDialog)
END_MESSAGE_MAP()


// CShowProcessDlg message handlers


void CShowProcessDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialog::OnCancel();
}


BOOL CShowProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
