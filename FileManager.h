
// FileManager.h : FileManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "type.h"

// CFileManagerApp:
// �йش����ʵ�֣������ FileManager.cpp
//

class CFileManagerApp : public CWinAppEx
{
public:
	CFileManagerApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

//����
	LanguageType language;
// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	CString LoadString(unsigned int ID);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFileManagerApp theApp;
