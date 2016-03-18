#pragma once
#include "SuperGridCtrl.h"
#include <list>
#include "fileprocess.h"
// CTempCleanerGrid

class CTempCleanerGrid : public CSuperGridCtrl
{
	DECLARE_DYNAMIC(CTempCleanerGrid)

public:
	CTempCleanerGrid();
	virtual ~CTempCleanerGrid();

	void InitializeGrid(void);
	void AdjustListColumnWidth();
	void ListFolders(std::list<TempCleanerInfo*>& cleaner_list, bool m_check_system, bool m_check_readonly, CString& Yes, CString& No);
	void DeleteSelect();

	virtual int GetIcon(const CTreeItem* pItem);
	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	

	//override this to set the color for current cell
	virtual COLORREF GetCellRGB(void){return GetSysColor(COLOR_HIGHLIGHT);};

	virtual BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	virtual BOOL IsSupprotDrag(void){return 0;};
	virtual void SortItem(CTreeItem** ppSortArray, int nChildren, int SubItem, BOOL bAscending);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CImageList m_image;//Must provide an imagelist

	//helper compare fn used with Quicksort
	static int CompareFileSizeString(const void* p1, const void* p2);
};


