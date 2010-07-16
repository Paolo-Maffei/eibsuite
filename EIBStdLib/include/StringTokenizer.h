#ifndef __STRINGTOKENIZER_HEADER__
#define __STRINGTOKENIZER_HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "CString.h"
#include "EibStdLib.h"

using namespace std;

class EIB_STD_EXPORT StringTokenizer
{
public:
    StringTokenizer(const CString& str, const CString& _delim);
   ~StringTokenizer(){};

    int         CountTokens();
    bool        HasMoreTokens();
    CString		NextToken();
    int         NextIntToken();
    double      NextFloatToken();
	int64		NextInt64Token();
    CString NextToken(const CString& delim);
    CString RemainingString();
    CString FilterNextToken(const CString& filterStr);

private:
    CString  _token_str;
    CString  _delim;

};

#endif
