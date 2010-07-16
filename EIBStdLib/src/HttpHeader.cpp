#include "HttpHeader.h"

CHttpHeader::CHttpHeader(void)
{
}

CHttpHeader::CHttpHeader(const CString& val)
{
	Parse(val);
}

CHttpHeader::~CHttpHeader(void)
{
}

const CString& CHttpHeader::GetName(void) const
{
	return _name;
}

const CString& CHttpHeader::GetValue(void) const
{
	return _value;
}

void CHttpHeader::Parse(const CString& val)
{
	int pos = val.FindFirstOf(HEADER_SEPERATOR);

	_name = val.SubString(0,pos);
	_value = val.SubString(pos + 1,val.GetLength() - pos - 1);
}

