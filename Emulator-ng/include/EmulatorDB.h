#ifndef __EMULATOR_DB_HEADER__
#define __EMULATOR_DB_HEADER__

#include "CString.h"
#include "GenericDB.h"
#include "EIBAddress.h"
#include "EmulatorConfig.h"
#include "CCemi_L_Data_Frame.h"

using namespace EibStack;

#define EMULATOR_DB_FILE "Emulator.db"
#define GROUP_VAL	  "VALUE"

#define MAX_EIB_VAL	14

class CGroupEntry
{
public:
	CGroupEntry();
	CGroupEntry(const CGroupEntry& rhs);
	virtual ~CGroupEntry();

	const CEibAddress& GetAddress() const { return _address; }
	short GetValueLen() const { return _val_len; }
	const char* GetValue() const { return _val; }

	void SetAddress(const CEibAddress& addr) { _address = addr; }
	void SetValueLen(short len) { _val_len = len; }
	void SetValue(const char* val, int len) { memcpy(_val, val, len); }

	void Reset();

	CGroupEntry& operator=(const CGroupEntry& rhs);
	bool operator==(const CGroupEntry& rhs);
	void Print() const;

private:
	CEibAddress _address;
	short _val_len;
	char _val[MAX_EIB_VAL];
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

	unsigned char* GetValueForGroup(const CEibAddress& address, int& len);
	void SetValueForGroup(const CEibAddress& address, const CCemi_L_Data_Frame& cemi);

	void Print() const;

private:

};

#endif
