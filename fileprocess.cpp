#include "stdafx.h"
#include "FileManager.h"
#include "fileprocess.h"

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