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

	virtual void HashString(const CString& input, CString& hash);
	virtual void HashFile(const CString& file_name, CString& hash);

	virtual bool Decode(const CString& cipher, CString& result);

	//virtual CString HashString(const char* input);
	//virtual CString HashFile(const char* file_name);

private:
	void HashString_MD5(const CString& input, CString& hash);
	void HashFile_MD5(const CString& file_name, CString& hash);
	void ConvertToString(unsigned char* buffer, CString& hash);

protected:
	DigestAlgorithm _algorithm;
};

#endif
