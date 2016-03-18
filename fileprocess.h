#pragma once
#include <list>
#include <time.h>
#include <fstream>

bool OpenDirectory(HWND owner, CString& path);
bool OpenDirectorySelectFile(HWND owner, CString& pathname);

CString ShowFolderBrowse(HWND owner=NULL);


typedef BOOL (WINAPI *EnumerateFunc) (LPCSTR lpFileOrPath, void* pUserData); 
void doFileEnumeration(LPSTR lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, void* pUserData);

//---------------------------------------------------------Folder-------------------------------------------------------------------
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
void ExportFoldersToFile(CString& path, 
							CString& FolderName, CString& FullPath, CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
							std::list<EmptyFolderInfo*>& empty_file_list, ExportFoldersToFileFunc pFunc, void* pUserData);

//---------------------------------------------------------File-------------------------------------------------------------------
struct EmptyFileInfo
{
	EmptyFileInfo(void)
		: FilePath("")
		, FileSize(0)
		, CreateTime(CTime::GetCurrentTime())
		, AccessTime(CTime::GetCurrentTime())
		, WriteTime(CTime::GetCurrentTime())
		, IsSystem(false)
		, IsReadOnly(false)
		, IsDisplay(true)
	{
	};
	EmptyFileInfo(CString& Path, unsigned int Size, CTime& CreateT, CTime& AccessT, CTime& WriteT, bool System, bool ReadOnly)
		: FilePath(Path)
		, FileSize(Size)
		, CreateTime(CreateT)
		, AccessTime(AccessT)
		, WriteTime(WriteT)
		, IsSystem(System)
		, IsReadOnly(ReadOnly)
		, IsDisplay(true)
	{
	};
	~EmptyFileInfo(void){};

	CString FilePath;
	unsigned int FileSize;
	CTime CreateTime;
	CTime AccessTime;
	CTime WriteTime;
	bool IsSystem;
	bool IsReadOnly;

	bool IsDisplay;
};

typedef BOOL (WINAPI *EmptyFileEnumerateFunc) (CString& FolderPath, unsigned int FileSize, CTime& CreateTime, CTime& AccessTime, CTime& WriteTime, bool IsSystem, bool IsReadOnly, void* pUserData); 
int doEmptyFileEnumeration(CString lpPath, EmptyFileEnumerateFunc pFunc, void* pUserData);

typedef BOOL (WINAPI *ExportFilesToFileFunc) (CString& FolderPath, void* pUserData); 
void ExportFilesToFile(CString& path, 
							CString& FolderName, CString& FullPath, CString& FileSize, CString& FileExt, CString& CreateTime, CString& AccessTime, CString& WriteTime,
								CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
							std::list<EmptyFileInfo*>& empty_file_list, ExportFilesToFileFunc pFunc, void* pUserData);


//----------------------------------------------------------------------TempCleaner---------------------------------------------------------------------
class TempCleanerInfo
{
public:
	TempCleanerInfo(void)
		: m_folder_path("")
		, m_ext("")
		, m_check(true)
	{
	};

	TempCleanerInfo(CString& Path, CString& ext, bool check)
		: m_folder_path(Path)
		, m_ext(ext)
		, m_check(check)
	{
	};

	~TempCleanerInfo(void)
	{
		DestoryAll();
	};

	void DestoryAll()
	{
		for (auto list_Iter = m_file_list.begin(); list_Iter != m_file_list.end(); ++list_Iter) 
		{
				delete (*list_Iter);
				(*list_Iter) = NULL;
		}
		m_file_list.erase(m_file_list.begin(), m_file_list.end());
		m_file_list.clear();
	}

	std::list<EmptyFileInfo*> m_file_list;
	CString m_folder_path;
	CString m_ext;
	bool m_check;
};

typedef BOOL (WINAPI *TempFileEnumerateFunc) (CString& FolderPath, void* pUserData); 
int doTempFileEnumeration(std::list<TempCleanerInfo*>& cleaner_list, TempFileEnumerateFunc pFunc, void* pUserData);

typedef BOOL (WINAPI *ExportTempCleanerToFileFunc) (CString& FolderPath, void* pUserData); 
void ExportTempCleanerToFile(CString& path, 
								CString& FolderName, CString& FullPath, CString& FileSize, CString& CreateTime, CString& AccessTime, CString& WriteTime,
								CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
							std::list<TempCleanerInfo*>& cleaner_list, ExportTempCleanerToFileFunc pFunc, void* pUserData);


CString CTime2String(CTime time);
CString FileSize2String(unsigned int FileSize);
unsigned int String2FileSize(CString str);