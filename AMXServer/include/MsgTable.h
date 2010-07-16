#ifndef __MSG_TABLE_HEADER__
#define __MSG_TABLE_HEADER__

#include "CString.h"
#include "TranslationTable.h"
#include "GenericDB.h"

#define AMX_MSG_TABLE_GENERAL_BLOCK "MESSAGES"

class AMXENTRY
{
public:
	unsigned char _device_id;
	bool _press;
	bool _release;

	bool operator<(const AMXENTRY& rhs) const;
	bool operator>(const AMXENTRY& rhs) const;
	bool operator==(const AMXENTRY& rhs) const;
};

class EIBENTRY
{
public:	
	unsigned short _function;
	unsigned char _value;

	bool operator<(const EIBENTRY& rhs) const;
	bool operator>(const EIBENTRY& rhs) const;
	bool operator==(const EIBENTRY& rhs) const;
};


class CMsgTable : public CTanslationTable<AMXENTRY,EIBENTRY>
{
public:
	CMsgTable();
	virtual ~CMsgTable();
};

class CMsgsDB : public CGenericDB<CString,CString>
{
public:
	CMsgsDB();
	virtual ~CMsgsDB();

	virtual void OnReadParamComplete(CString& current_record, const CString& param,const CString& value);
	virtual void OnReadRecordComplete(CString& current_record);
	virtual void OnReadRecordNameComplete(CString& current_record, const CString& record_name);
	virtual void OnSaveRecordStarted(const CString& record,CString& record_name, map<CString,CString>& param_values);
};

#endif
