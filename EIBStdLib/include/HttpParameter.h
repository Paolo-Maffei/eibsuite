#ifndef __HTTP_PARAMETER_HEADER__
#define __HTTP_PARAMETER_HEADER__

#include "CString.h"

class EIB_STD_EXPORT CHttpParameter
{
public:
	CHttpParameter();
	CHttpParameter(const CString& val);
	CHttpParameter(const CString& name,const CString& value);
	virtual ~CHttpParameter();

	const CString& GetName() const;
	void SetName(const CString& name);

	const CString& GetValue() const;
	void SetValue(const CString& value);

private:
	void Parse(const CString& val);

private:
	CString _name;
	CString _value;
};

#endif

