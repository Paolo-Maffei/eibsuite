#include "EmulatorDB.h"
#include "Emulator-ng.h"

CGroupEntry::CGroupEntry() :
_address((unsigned int)0, true),
_val_len(0)
{
	memset(_val, 0, sizeof(_val));
}

CGroupEntry::CGroupEntry(const CGroupEntry& rhs)
{
	*this = rhs;
}

CGroupEntry::~CGroupEntry()
{
}

void CGroupEntry::Reset()
{
	_address.Set((unsigned int)0, true);
	_val_len = 0;
	memset(_val, 0, sizeof(_val));
}

void CGroupEntry::Print() const
{
	LOG_SCREEN("%s: %s", _address.ToString().GetBuffer(), CString::ToHexFormat(_val, _val_len, true).GetBuffer());
}

CGroupEntry& CGroupEntry::operator=(const CGroupEntry& rhs)
{
	_address = rhs._address;
	_val_len = rhs._val_len;
	memcpy(_val, rhs._val, rhs._val_len);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEmulatorDB::CEmulatorDB()
{

}

CEmulatorDB::~CEmulatorDB()
{

}

void CEmulatorDB::OnReadParamComplete(CGroupEntry& current_record, const CString& param,const CString& value)
{
	CString tmp(param);
	tmp.ToUpper();

	if(tmp == GROUP_VAL){
		char buffer[MAX_EIB_VAL];
		int len = value.ToByteArray(buffer, sizeof(buffer));
		current_record.SetValueLen(len);
		current_record.SetValue(buffer, len);

	}else{
		throw CEIBException(ConfigFileError,"Configuration file error. Unknown parameter: %s", param.GetBuffer());
	}
}

void CEmulatorDB::OnReadRecordComplete(CGroupEntry& current_record)
{
	if(current_record.GetValueLen() == 0){
		throw CEIBException(ConfigFileError,"Configuration file error. Group entry %s is missing value entry", current_record.GetAddress().ToString().GetBuffer());
	}
	if(!AddRecord(current_record.GetAddress().ToByteArray(), current_record)){
		throw CEIBException(ConfigFileError,"Configuration file error. Duplicate Group entry: %s", current_record.GetAddress().ToString().GetBuffer());
	}

	current_record.Reset();
}

void CEmulatorDB::OnReadRecordNameComplete(CGroupEntry& current_record, const CString& record_name)
{
	START_TRY
		CEibAddress addr(record_name);
		current_record.SetAddress(addr);
	END_TRY_START_CATCH(e)
		throw CEIBException(ConfigFileError,"Configuration file error. all blocks must have valid group address");
	END_CATCH
}

void CEmulatorDB::OnSaveRecordStarted(const CGroupEntry& record,CString& record_name, list<pair<CString, CString> >& param_values)
{
	record_name = record.GetAddress().ToString();
	param_values.push_front(pair<CString, CString>(GROUP_VAL,""));
}

unsigned char* CEmulatorDB::GetValueForGroup(const CEibAddress& address, int& len)
{
	static unsigned char current_value[MAX_EIB_VAL];

	map<int,CGroupEntry>::iterator it = _data.find(address.ToByteArray());
	if(it == _data.end()){
		len = 0;
		return NULL;
	}
	len = it->second.GetValueLen();
	memcpy(current_value, it->second.GetValue(), len);
	return current_value;
}

void CEmulatorDB::Print() const
{
	map<int,CGroupEntry>::const_iterator it;
	int i = 1;
	LOG_SCREEN("---------------------------------------\n");
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		LOG_SCREEN("%d. ", i++);
		it->second.Print();
		LOG_SCREEN("\n");
	}
	LOG_SCREEN("---------------------------------------");
}
