#ifndef __URL_ENCODING_HEADER__
#define __URL_ENCODING_HEADER__

#include "EibStdLib.h"
#include "CString.h"

class EIB_STD_EXPORT URLEncoder
{
public:
	static CString Encode(const CString& str);
	static CString Decode(const CString& str);
private:
	static bool IsOrdinaryChar(char c);
	static int ConvertToDec(const char* hex);
	static void GetAsDec(char* hex);
};

#endif
