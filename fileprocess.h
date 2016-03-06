#pragma once
#include <list>

CString ShowFolderBrowse(HWND owner=NULL);


typedef BOOL (WINAPI *EnumerateFunc) (LPCSTR lpFileOrPath, void* pUserData); 
void doFileEnumeration(LPSTR lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, void* pUserData);

struct EmptyFolderInfo
{
	EmptyFolderInfo(void)
		: FolderPath("")
		, IsSystem(false)
		, IsReadOnly(false)
	{
	};
	EmptyFolderInfo(CString& Path, bool System, bool ReadOnly)
		: FolderPath(Path)
		, IsSystem(System)
		, IsReadOnly(ReadOnly)
	{
	};
	~EmptyFolderInfo(void){};

	CString FolderPath;
	bool IsSystem;
	bool IsReadOnly;
};


typedef BOOL (WINAPI *EmptyFolderEnumerateFunc) (CString& FolderPath, bool IsSystem, bool IsReadOnly, void* pUserData); 
int doEmptyFolderEnumeration(CString lpPath, EmptyFolderEnumerateFunc pFunc, void* pUserData);
int doEmptyFolderEnumeration(CString lpPath, std::list<EmptyFolderInfo*>& m_empty_file_list);