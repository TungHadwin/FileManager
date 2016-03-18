#include "stdafx.h"
#include "FileManager.h"
#include "fileprocess.h"
#include <fstream>

bool OpenDirectory(HWND owner, CString& path)
{
	std::string shell;
	shell="/e,"+path;
	//打开指定目录  "/e,C:\\Windows"
	if(ShellExecute(owner, _T("open"), _T("explorer.exe"), shell.c_str(), NULL, SW_SHOWNORMAL)<=(HINSTANCE)32)
	{
		AfxMessageBox(theApp.LoadString(IDS_OpenDirectoryFailed)); //打开指定目录失败！
		return false;
	}
	return true;
}

bool OpenDirectorySelectFile(HWND owner, CString& pathname)
{ 
    std::string shell;
	shell="/select,"+pathname;
	//打开指定目录并选择指定文件  "/select,C:\\Windows\\desktop.ini"
	if(ShellExecute(owner, _T("open"), _T("explorer.exe"), shell.c_str(), NULL, SW_SHOWNORMAL)<=(HINSTANCE)32)
	{
		AfxMessageBox(theApp.LoadString(IDS_OpenDirectoryFailed)); //打开指定目录失败！
		return false;
	}
	return true;
}

CString ShowFolderBrowse(HWND owner)    
{
    char szPath[MAX_PATH];     //存放选择的目录路径 
    ZeroMemory(szPath, sizeof(szPath));  

    BROWSEINFO bi;   
    bi.hwndOwner = owner;   
    bi.pidlRoot = NULL;   
    bi.pszDisplayName = szPath;   
    bi.lpszTitle = "";   
    bi.ulFlags = 0;   
    bi.lpfn = NULL;   
    bi.lParam = 0;   
    bi.iImage = 0;   
    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	
	CString str("");
    if(lp && SHGetPathFromIDList(lp, szPath))   
        str = szPath;    
	return str; 
}

/*
lpPath		指定欲遍历的路径（文件夹）
bRecursion	指定是否递归处理子目录
bEnumFiles	指定是枚举文件还是枚举子目录
pFunc		为用户回调函数，枚举过程中每遇到一个文件或子目录，都会调用它，并传入这个文件或子目录的完整路径
pUserData	为用户任意指定的数据，它也将被传入用户回调函数。用户回调函数(EnumerateFunc)有两个参数，一个是文件或子目录的完整路径(lpFileOrPath)，一个是用户自定义数据(pUserData)，它被自动调用，用户需在此函数中编码处理代码。
*/

void doFileEnumeration(LPSTR lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, void* pUserData)  
{  
    static BOOL s_bUserBreak = FALSE;  
    try
	{  
        //-------------------------------------------------------------------------  
        if(s_bUserBreak) return;  
          
        int len = strlen(lpPath);  
        if(lpPath==NULL || len<=0) return;  
          
        char path[MAX_PATH];  
        strcpy(path, lpPath);  
        if(lpPath[len-1] != '\\') strcat(path, "\\");  
        strcat(path, "*");  
          
        WIN32_FIND_DATA fd;  
        HANDLE hFindFile = FindFirstFile(path, &fd);  
        if(hFindFile == INVALID_HANDLE_VALUE)  
        {  
            ::FindClose(hFindFile); return;  
        }  
          
        char tempPath[MAX_PATH]; BOOL bUserReture=TRUE; BOOL bIsDirectory;  
          
        BOOL bFinish = FALSE;  
        while(!bFinish)  
        {  
            strcpy(tempPath, lpPath);  
            if(lpPath[len-1] != '\\') strcat(tempPath, "\\");  
            strcat(tempPath, fd.cFileName);  
              
            bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  
              
            //如果是.或..  
            if( bIsDirectory  
                && (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0))   
            {         
                bFinish = (FindNextFile(hFindFile, &fd) == FALSE);  
                continue;  
            }  
              
            if(pFunc && bEnumFiles!=bIsDirectory)  
            {  
                bUserReture = pFunc(tempPath, pUserData);  
                if(bUserReture==FALSE)  
                {  
                    s_bUserBreak = TRUE; ::FindClose(hFindFile); return;  
                }  
            }  
                         
            if(bIsDirectory && bRecursion) //是子目录  
            {  
                doFileEnumeration(tempPath, bRecursion, bEnumFiles, pFunc, pUserData);  
            }  
              
            bFinish = (FindNextFile(hFindFile, &fd) == FALSE);  
        }  
          
        ::FindClose(hFindFile);  
          
        //-------------------------------------------------------------------------  
    }catch(...){ ASSERT(0); return; }  
}  

int doEmptyFolderEnumeration(CString lpPath, EmptyFolderEnumerateFunc pFunc, void* pUserData)  
{  
	lpPath+="\\*.*";

	int filenum=0;
    CFileFind  findfile;
	BOOL m_flag = findfile.FindFile(lpPath);
    while(m_flag) 
    {  
		m_flag = findfile.FindNextFile(); 
		if(findfile.IsDirectory())   //如果是目录跳过
		{
			if((!findfile.IsDots()))
			{
				CString filename = findfile.GetFilePath();
			
				int num = doEmptyFolderEnumeration(filename, pFunc, pUserData);
				if(num==0)
					pFunc(filename, findfile.IsSystem(), findfile.IsReadOnly(), pUserData);
				filenum+=num;
			}
			continue;
		}
		else
			filenum++;
	}
	return filenum;
}  

int doEmptyFolderEnumeration(CString lpPath, std::list<EmptyFolderInfo*>& m_empty_file_list)
{
	lpPath+="\\*.*";

	int filenum=0;
    CFileFind  findfile;
	BOOL m_flag = findfile.FindFile(lpPath);
    while(m_flag) 
    {  
		m_flag = findfile.FindNextFile(); 
		if(findfile.IsDirectory())   //如果是目录跳过
		{
			if((!findfile.IsDots()))
			{
				CString filename = findfile.GetFilePath();
			
				int num = doEmptyFolderEnumeration(filename, m_empty_file_list);
				if(num==0)
				{
					EmptyFolderInfo* info = new EmptyFolderInfo(filename, findfile.IsSystem(), findfile.IsReadOnly());
					m_empty_file_list.push_back(info);//pFunc(filename, findfile.IsSystem(), findfile.IsReadOnly(), /*findfile.IsTemporary(),*/ pUserData);
				}
				filenum+=num;
			}
			continue;
		}
		else
			filenum++;
	}
	return filenum;
}

void ExportFoldersToFile(CString& path, CString& FolderName, CString& FullPath, CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
							std::list<EmptyFolderInfo*>& empty_file_list, ExportFoldersToFileFunc pFunc, void* pUserData)
{
	std::fstream file(path, std::fstream::out);
	file<<FolderName<<','<<FullPath<<','<<SystemFile<<','<<ReadOnly<<"\n";

	for (auto list_Iter = empty_file_list.begin(); list_Iter != empty_file_list.end(); ++list_Iter) 
	{
		if((*list_Iter)->IsDisplay)
		{
			int pos = (*list_Iter)->FolderPath.ReverseFind('\\');
			file<<(*list_Iter)->FolderPath.Right((*list_Iter)->FolderPath.GetLength()-pos-1)<<','\
				<<(*list_Iter)->FolderPath<<','\
				<<((*list_Iter)->IsSystem? Yes:No)<<','\
				<<((*list_Iter)->IsReadOnly? Yes:No)<<"\n";

			pFunc((*list_Iter)->FolderPath, pUserData);
		}
	}
}

int doEmptyFileEnumeration(CString lpPath, EmptyFileEnumerateFunc pFunc, void* pUserData)  
{  
	lpPath+="\\*.*";

    CFileFind  findfile;
	BOOL m_flag = findfile.FindFile(lpPath);
    while(m_flag) 
    {  
		m_flag = findfile.FindNextFile(); 
		if(findfile.IsDirectory())   //如果是目录跳过
		{
			if((!findfile.IsDots()))
			{
				CString filename = findfile.GetFilePath();
				doEmptyFileEnumeration(filename, pFunc, pUserData);
			}
			continue;
		}
		else
		{
			if(findfile.GetLength()==0)
			{
				CTime CreateTime;
				CTime AccessTime;
				CTime WriteTime;
				findfile.GetCreationTime(CreateTime);
				findfile.GetLastAccessTime(AccessTime);
				findfile.GetLastWriteTime(WriteTime);
				pFunc(findfile.GetFilePath(), 0, CreateTime, AccessTime, WriteTime, findfile.IsSystem(), findfile.IsReadOnly(), pUserData);
			}
		}
	}
	return 0;
}  

void ExportFilesToFile(CString& path,
							CString& FolderName, CString& FullPath, CString& FileSize, CString& FileExt, CString& CreateTime, CString& AccessTime, CString& WriteTime,
								CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
							std::list<EmptyFileInfo*>& empty_file_list, ExportFilesToFileFunc pFunc, void* pUserData)
{
	std::fstream file(path, std::fstream::out);
	file<<FolderName<<','<<FullPath<<','<<FileSize<<','<<FileExt<<','<<CreateTime<<','<<AccessTime<<','<<WriteTime<<','<<SystemFile<<','<<ReadOnly<<"\n";

	for (auto list_Iter = empty_file_list.begin(); list_Iter != empty_file_list.end(); ++list_Iter) 
	{
		if((*list_Iter)->IsDisplay)
		{
			int pos = (*list_Iter)->FilePath.ReverseFind('\\');
			int pos_ext = (*list_Iter)->FilePath.ReverseFind('.');
			file<<(*list_Iter)->FilePath.Right((*list_Iter)->FilePath.GetLength()-pos-1)<<','\
				<<(*list_Iter)->FilePath<<','\
				<<(*list_Iter)->FileSize<<"B,"\
				<<(*list_Iter)->FilePath.Right((*list_Iter)->FilePath.GetLength()-pos_ext-1)<<','\
				<<CTime2String((*list_Iter)->CreateTime)<<','\
				<<CTime2String((*list_Iter)->AccessTime)<<','\
				<<CTime2String((*list_Iter)->WriteTime)<<','\
				<<((*list_Iter)->IsSystem? Yes:No)<<','\
				<<((*list_Iter)->IsReadOnly? Yes:No)<<"\n";

			pFunc((*list_Iter)->FilePath, pUserData);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------
int doTempFileEnumeration(CString lpPath, CString& ext, std::list<EmptyFileInfo*>& file_list, TempFileEnumerateFunc pFunc, void* pUserData)  
{  
	lpPath+="\\*.*";

    CFileFind  findfile;
	BOOL m_flag = findfile.FindFile(lpPath);
    while(m_flag) 
    {  
		m_flag = findfile.FindNextFile(); 
		if(findfile.IsDirectory())   //如果是目录跳过
		{
			if((!findfile.IsDots()))
			{
				CString filename = findfile.GetFilePath();
				doTempFileEnumeration(filename, ext, file_list, pFunc, pUserData);
			}
			continue;
		}
		else
		{
			CString filename = findfile.GetFileName();
			int pos = filename.ReverseFind('.');
			filename = filename.Right(filename.GetLength()-pos-1);

			if(filename==ext)
			{
				CTime CreateTime;
				CTime AccessTime;
				CTime WriteTime;
				findfile.GetCreationTime(CreateTime);
				findfile.GetLastAccessTime(AccessTime);
				findfile.GetLastWriteTime(WriteTime);

				EmptyFileInfo* info = new EmptyFileInfo(findfile.GetFilePath(), findfile.GetLength(), CreateTime, AccessTime, WriteTime, findfile.IsSystem(), findfile.IsReadOnly());
				file_list.push_back(info);

				pFunc(findfile.GetFilePath(), pUserData);
			}
		}
	}
	return 0;
} 

int doTempFileEnumeration(std::list<TempCleanerInfo*>& cleaner_list, TempFileEnumerateFunc pFunc, void* pUserData)  
{
	for (auto list_Iter = cleaner_list.begin(); list_Iter != cleaner_list.end(); ++list_Iter)
	{
		if((*list_Iter)->m_check)
			doTempFileEnumeration((*list_Iter)->m_folder_path, (*list_Iter)->m_ext, (*list_Iter)->m_file_list, pFunc, pUserData);
	}
	return 0;
}

void ExportTempCleanerToFile(CString& path, 
								CString& FolderName, CString& FullPath, CString& FileSize, CString& CreateTime, CString& AccessTime, CString& WriteTime,
								CString& SystemFile, CString& ReadOnly, CString& Yes, CString& No,
									std::list<TempCleanerInfo*>& cleaner_list, ExportTempCleanerToFileFunc pFunc, void* pUserData)
{
	std::fstream file(path, std::fstream::out);
	file<<FolderName<<','<<FullPath<<','<<FileSize<<','<<CreateTime<<','<<AccessTime<<','<<WriteTime<<','<<SystemFile<<','<<ReadOnly<<"\n";

	for (auto cleaner_list_Iter = cleaner_list.begin(); cleaner_list_Iter != cleaner_list.end(); ++cleaner_list_Iter) 
	{
		for (auto list_Iter = (*cleaner_list_Iter)->m_file_list.begin(); list_Iter != (*cleaner_list_Iter)->m_file_list.end(); ++list_Iter) 
		{
			if((*list_Iter)->IsDisplay)
			{
				int pos = (*list_Iter)->FilePath.ReverseFind('\\');
				file<<(*list_Iter)->FilePath.Right((*list_Iter)->FilePath.GetLength()-pos-1)<<','\
					<<(*list_Iter)->FilePath<<','\
					<<FileSize2String((*list_Iter)->FileSize)<<','\
					<<CTime2String((*list_Iter)->CreateTime)<<','\
					<<CTime2String((*list_Iter)->AccessTime)<<','\
					<<CTime2String((*list_Iter)->WriteTime)<<','\
					<<((*list_Iter)->IsSystem? Yes:No)<<','\
					<<((*list_Iter)->IsReadOnly? Yes:No)<<"\n";

				pFunc((*list_Iter)->FilePath, pUserData);
			}
		}
	}
}


CString CTime2String(CTime time)
{
	CString str;
	str.Format("%04d/%02d/%02d %02d:%02d:%02d",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return str;
}

CString FileSize2String(unsigned int FileSize)
{
	CString str;
	if(FileSize>=1024*1024)
		str.Format("%0.2fMB",FileSize/1024.0/1024.0);
	else if(FileSize>=1024)
		str.Format("%0.2fKB",FileSize/1024.0);
	else
		str.Format("%dB",FileSize);
	return str;
}

unsigned int String2FileSize(CString str)
{
	char *pbuf=str.GetBuffer();
	double num=atof(pbuf);
	str.ReleaseBuffer();

	if((pbuf[str.GetLength()-2]=='K'))
		num*=1000;
	else if((pbuf[str.GetLength()-2]=='M'))
		num*=1000000;
	return num;
}