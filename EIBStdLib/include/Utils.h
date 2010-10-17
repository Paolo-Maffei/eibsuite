#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include <fstream>
#include <time.h>
#include "EibStdLib.h"
#include "CString.h"
#include "Globals.h"

#ifdef WIN32
//windows
#include <direct.h>
#include <Iphlpapi.h>
#else
//linux
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include <errno.h>
#include <net/if.h>
#include <stdlib.h>
#endif

using namespace std;

class EIB_STD_EXPORT CUtils
{
public:

	static void ReadFile(const CString& full_name, CString& content);
	static void SaveFile(const CString& full_name, const CString& content);
	static int GetFileSize(const CString& full_name);
	static void GetTimeStrForFile(CString& time_str);
	static void GetCurrentPath(CString& path);
	static int GetNumOfNICs();
	static bool EnumNics(map<CString,CString>& nics);
	static void WaitForCharInput(char expected, const CString& msg, bool sleep = false);

	static void WaitForInput(CString& input, const CString& msg, bool sleep = false);

private:
	CUtils(){};
	virtual ~CUtils() {};
};

template<class T>
CString ToHex(const T& num)
{
	ostringstream os;
	os << hex << num;
	return os.str();
}

#endif

