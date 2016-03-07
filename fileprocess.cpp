#include "stdafx.h"
#include "FileManager.h"
#include "fileprocess.h"
#include <fstream>

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
		if(findfile.IsDirectory())   //�����Ŀ¼����
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
	file<<FolderName<<','<<FullPath<<','<<FileSize<<','<<FileExt<<','<<CreateTime<<','<<AccessTime<<','<<AccessTime<<','<<WriteTime<<','<<ReadOnly<<"\n";

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

CString CTime2String(CTime time)
{
	CString str;
	str.Format("%04d/%02d/%02d %02d:%02d:%02d",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return str;
}