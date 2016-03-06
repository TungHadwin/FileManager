#include "stdafx.h"
#include "FileManager.h"
#include "fileprocess.h"

bool OpenDirectory(HWND owner, CString& path)
{
	std::string shell;
	shell="/e,"+path;
	//��ָ��Ŀ¼  "/e,C:\\Windows"
	if(ShellExecute(owner, _T("open"), _T("explorer.exe"), shell.c_str(), NULL, SW_SHOWNORMAL)<=(HINSTANCE)32)
	{
		AfxMessageBox(theApp.LoadString(IDS_OpenDirectoryFailed)); //��ָ��Ŀ¼ʧ�ܣ�
		return false;
	}
	return true;
}

bool OpenDirectorySelectFile(HWND owner, CString& pathname)
{ 
    std::string shell;
	shell="/select,"+pathname;
	//��ָ��Ŀ¼��ѡ��ָ���ļ�  "/select,C:\\Windows\\desktop.ini"
	if(ShellExecute(owner, _T("open"), _T("explorer.exe"), shell.c_str(), NULL, SW_SHOWNORMAL)<=(HINSTANCE)32)
	{
		AfxMessageBox(theApp.LoadString(IDS_OpenDirectoryFailed)); //��ָ��Ŀ¼ʧ�ܣ�
		return false;
	}
	return true;
}

CString ShowFolderBrowse(HWND owner)    
{
    char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
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
    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);  

	
	CString str("");
    if(lp && SHGetPathFromIDList(lp, szPath))   
        str = szPath;    
	return str; 
}

/*
lpPath		ָ����������·�����ļ��У�
bRecursion	ָ���Ƿ�ݹ鴦����Ŀ¼
bEnumFiles	ָ����ö���ļ�����ö����Ŀ¼
pFunc		Ϊ�û��ص�������ö�ٹ�����ÿ����һ���ļ�����Ŀ¼�����������������������ļ�����Ŀ¼������·��
pUserData	Ϊ�û�����ָ�������ݣ���Ҳ���������û��ص��������û��ص�����(EnumerateFunc)������������һ�����ļ�����Ŀ¼������·��(lpFileOrPath)��һ�����û��Զ�������(pUserData)�������Զ����ã��û����ڴ˺����б��봦����롣
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
              
            //�����.��..  
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
                         
            if(bIsDirectory && bRecursion) //����Ŀ¼  
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
		if(findfile.IsDirectory())   //�����Ŀ¼����
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
		if(findfile.IsDirectory())   //�����Ŀ¼����
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