#include "Directory.h"

CDirectory::CDirectory()
{
}

CDirectory::~CDirectory()
{
}

bool CDirectory::Create(const CString& path)
{
	int res = 0;

#ifdef WIN32
	 res = _mkdir(path.GetBuffer());
#else
	res = mkdir(path.GetBuffer(),RW_DIR_MASK);
#endif
	 return (res != -1);
}

bool CDirectory::Change(const CString& path)
{
	int res = 0;
#ifdef WIN32
	res = _chdir(path.GetBuffer());
#else
	res = chdir(path.GetBuffer());
#endif
	return (res != -1);
}

bool CDirectory::Delete(const CString& path)
{
	return true;
}

CString CDirectory::CurrentDirectory()
{
	char current_path[_MAX_PATH];
#ifdef WIN32	
	CString ret(_getcwd(current_path,_MAX_PATH));
#else
	CString ret(getcwd(current_path,_MAX_PATH));
#endif
	return ret;
}

bool CDirectory::IsExist(const CString& path)
{
	if (access( path.GetBuffer(), 0 ) == 0 )
	{
		struct stat status;
	    stat( path.GetBuffer(), &status );
        return (status.st_mode & S_IFDIR);
	}
	return false;
}
