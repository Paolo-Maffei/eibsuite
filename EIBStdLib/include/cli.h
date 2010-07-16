#ifndef __CLI_HEADER__
#define __CLI_HEADER__

#include "EibStdLib.h"
#include "CException.h"
using namespace std;

class EIB_STD_EXPORT ConsoleCLI
{
public:
	ConsoleCLI() {};
	~ConsoleCLI() {};

	static bool Getbool(const CString& msg, bool& val, bool def);
	static bool Getint(const CString& msg, int& val, int def);
	static bool GetCString(const CString& msg, CString& val, const CString& def);
	static bool GetStrOption(const CString& msg, const map<CString,CString>& options, CString& val, const CString& def);
	static bool GetStrOption(const CString& msg, const map<int,CString>& options, int& val, const int& def);

};

#endif
