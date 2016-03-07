#pragma once
#include <list>

bool OpenDirectory(HWND owner, CString& path);
bool OpenDirectorySelectFile(HWND owner, CString& pathname);

CString ShowFolderBrowse(HWND owner=NULL);


typedef BOOL (WINAPI *EnumerateFunc) (LPCSTR lpFileOrPath, void* pUserData); 
void doFileEnumeration(LPSTR lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, void* pUserData);

struct EmptyFolderInfo
{
	EmptyFolderInfo(void)
		: FolderPath("")
		, IsSystem(false)
		, IsReadOnly(false)
		, IsDisplay(true)
	{
	};
	EmptyFolderInfo(CString& Path, bool System, bool ReadOnly)
		: FolderPath(Path)
		, IsSystem(System)
		, IsReadOnly(ReadOnly)
		, IsDisplay(true)
	{
	};
	~EmptyFolderInfo(void){};

	CString FolderPath;
	bool IsSystem;
	bool IsReadOnly;

	bool IsDisplay;
};


typedef BOOL (WINAPI *EmptyFolderEnumerateFunc) (CString& FolderPath, bool IsSystem, bool IsReadOnly, void* pUserData); 
int doEmptyFolderEnumeration(CString lpPath, EmptyFolderEnumerateFunc pFunc, void* pUserData);
int doEmptyFolderEnumeration(CString lpPath, std::list<EmptyFolderInfo*>& m_empty_file_list);

typedef BOOL (WINAPI *ExportFoldersToFileFunc) (CString& FolderPath, void* pUserData); 
void ExportFoldersToFile(CString& path, CString& FolderName, CString& FullPath, CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
							std::list<EmptyFolderInfo*>& empty_file_list, ExportFoldersToFileFunc pFunc, void* pUserData);