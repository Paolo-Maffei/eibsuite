#ifndef __EMULATOR_DB_HEADER__
#define __EMULATOR_DB_HEADER__

#include "CString.h"
#include "GenericDB.h"
#include "EIBAddress.h"
#include "EmulatorConfig.h"

using namespace EibStack;

#define EMULATOR_DB_FILE "Emulator.db"
#define GROUP_BLOCK_NAME_PREFIX "GROUP-"

class CGroupEntry
{
public:
	CGroupEntry();
	virtual ~CGroupEntry();

private:
	CEibAddress _address;
	short _val_len;
	char _default_val[14];
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

	void Print();

private:

};

#endif
