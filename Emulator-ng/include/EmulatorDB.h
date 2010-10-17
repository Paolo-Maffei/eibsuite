#ifndef __EMULATOR_DB_HEADER__
#define __EMULATOR_DB_HEADER__

#include "CString.h"
#include "GenericDB.h"
#include "EIBAddress.h"
#include "EmulatorConfig.h"

using namespace EibStack;

#define EMULATOR_DB_FILE "Emulator.db"
#define GROUP_VAL	  "VALUE"

class CGroupEntry
{
public:
	CGroupEntry();
	CGroupEntry(const CGroupEntry& rhs);
	virtual ~CGroupEntry();

	const CEibAddress& GetAddress() const { return _address; }
	short GetValueLen() const { return _val_len; }

	void SetAddress(const CEibAddress& addr) { _address = addr; }
	void SetValueLen(short len) { _val_len = len; }
	void SetValue(char* val, int len) { memcpy(_val, val, len); }

	void Reset();

	CGroupEntry& operator=(const CGroupEntry& rhs);
	bool operator==(const CGroupEntry& rhs);
	void Print() const;

private:
	CEibAddress _address;
	short _val_len;
	char _val[14];
};

class CEmulatorDB : public CGenericDB<int,CGroupEntry>
{
public:
	CEmulatorDB();
	virtual ~CEmulatorDB();

	virtual void OnReadParamComplete(CGroupEntry& current_record, const CString& param,const CString& value);
	virtual void OnReadRecordComplete(CGroupEntry& current_record);
	virtual void OnReadRecordNameComplete(CGroupEntry& current_record, const CString& record_name);
	virtual void OnSaveRecordStarted(const CGroupEntry& record,CString& record_name, list<pair<CString, CString> >& param_values);

	void Print() const;

private:

};

#endif
