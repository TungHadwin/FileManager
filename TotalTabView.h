#pragma once
#include "FileManagerDoc.h"
#include "MultTabView.h"

// CTotalTabView view

class CTotalTabView : public CMultTabView
{
	DECLARE_DYNCREATE(CTotalTabView)

public:
	void LoadRegedits();
	void SaveRegedits();

	void ShowCtrlPanel(bool en);
protected:
	CTotalTabView();           // protected constructor used by dynamic creation
	virtual ~CTotalTabView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CFileManagerDoc* GetDocument() const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
protected:
	DECLARE_MESSAGE_MAP()

private:
	void ActiveEmptyFileManagerTabCtrl();
	
};


