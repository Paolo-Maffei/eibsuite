#ifndef __DIRCETORY_HEADER__
#define __DIRCETORY_HEADER__

#include "EibStdLib.h"
#include "CString.h"

#define RW_DIR_MASK 0777

#ifndef _MAX_PATH
#define _MAX_PATH 256
#endif

class EIB_STD_EXPORT CDirectory
{
public:
	CDirectory();
	virtual ~CDirectory();

	static bool Create(const CString& path);
	static bool Change(const CString& path);
	static bool Delete(const CString& path);

	static bool IsExist(const CString& path);

	static CString CurrentDirectory();
};

#endif
