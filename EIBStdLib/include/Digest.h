#ifndef __DIGEST_HEADER__
#define __DIGEST_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "Globals.h"
#include "MD5.h"
#include "Base64.h"

enum EIB_STD_EXPORT DigestAlgorithm
{
	ALGORITHM_MD5,
	ALGORITHM_BASE64
};

class EIB_STD_EXPORT CDigest
{
public:
	CDigest(DigestAlgorithm algorithm = ALGORITHM_MD5);
	virtual ~CDigest();

	virtual CString HashString(const CString& input);
	virtual CString HashFile(const CString& file_name);

	virtual bool Decode(const CString& cipher, CString& result);

	//virtual CString HashString(const char* input);
	//virtual CString HashFile(const char* file_name);

private:
	CString HashString_MD5(const CString& input);
	CString HashFile_MD5(const CString& file_name);
	CString ConvertToString(unsigned char* buffer);

protected:
	DigestAlgorithm _algorithm;
};

#endif
