#include "HttpParameter.h"

CHttpParameter::CHttpParameter()
{
}

CHttpParameter::CHttpParameter(const CString& val)
{
	Parse(val);
}

CHttpParameter::CHttpParameter(const CString& name,const CString& value)
{
	SetName(name);
	SetValue(value);
}

CHttpParameter::~CHttpParameter()
{
}

const CString& CHttpParameter::GetName() const
{
	return _name;
}

void CHttpParameter::SetName(const CString& name)
{
	_name = name;
}

const CString& CHttpParameter::GetValue() const
{
	return _value;
}

void CHttpParameter::SetValue(const CString& value)
{
	_value = value;
}

void CHttpParameter::Parse(const CString& val)
{
	int pos = val.FindFirstOf('=');
	//check validity here
	if(pos == -1)
	{
		_name.Clear();
		_value.Clear();
		return;
	}

	_name = val.SubString(0,pos);
	_value = val.SubString(pos + 1,val.GetLength() - pos - 1);
}

