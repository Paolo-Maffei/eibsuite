#ifndef __BASE_64_HEADER__
#define __BASE_64_HEADER__

#include <iostream>
#include "CString.h"

static const CString base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

class CBase64
{
public:
	CBase64();
	virtual ~CBase64();

	static CString Encode(const CString& clear);
	static CString Decode(const CString& cipher);

private:
	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
};

#endif
