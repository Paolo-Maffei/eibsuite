#ifndef __USER_ENTRY_HEADER__
#define __USER_ENTRY_HEADER__

#include "CString.h"
#include "GenericDB.h"

#define SMS_DB_FILE "Sms.db"
#define PHONE_NUMBER_BLOCK_PREFIX_STR "PHONE_NUMBER-"
#define ALERT_PARAM_STR "EIB_TO_SMS"
#define SMS_COMMAND_STR "SMS_TO_EIB"
#define ALERT_PARAM_SEPERATOR_STR ":"
#define CMD_PARAM_SEPERATO_STR ":"

struct _AddressValueKey
{

	_AddressValueKey(unsigned short d_address,unsigned short value)
	{
		_d_address = d_address;
		_value = value;
	}

	_AddressValueKey(const _AddressValueKey& rhs)
	{
		_d_address = rhs._d_address;
		_value = rhs._value;
	}

	bool operator == (const _AddressValueKey& avk) const
	{
		if ( _d_address == avk._d_address && _value == avk._value){
			return true;
		}
		return false;
	}

	bool operator < (const _AddressValueKey& avk) const
	{
		if (_d_address < avk._d_address && _value < avk._value){
			return true;
		}
		return false;
	}

	bool operator > (const _AddressValueKey& avk) const
	{
		return (_d_address > avk._d_address || _value > avk._value);
	}

	unsigned short _d_address;
	unsigned short _value;
};

typedef struct _AddressValueKey AddressValueKey;

class CUserAlertRecord
{
public:
	CUserAlertRecord() {};
	virtual ~CUserAlertRecord() {};

	unsigned char GetValue() const{ return _value;}
	unsigned short GetDestAddress() const { return _d_address;}
	const CString& GetPoneNumber() const{ return _phone_number; }
	const CString& GetTextMessage() const{ return _text_msg;}

	void SetDestAddress(unsigned short address) { _d_address = address;}
	void SetValue(unsigned char value) { _value = value;}
	void SetPhoneNumber(const CString& phone_number) { _phone_number = phone_number; }
	void SetTextMessage(const CString& text_message) { _text_msg = text_message;}

	bool operator==(const CUserAlertRecord& other)
	{
		return (_value == other._value && _d_address == other._d_address);
	}

private:
	unsigned short _d_address;
	unsigned char _value;
	CString _text_msg;
	CString _phone_number;
};

typedef CUserAlertRecord CCommandRecord;

class CUserEntry
{
public:
	CUserEntry() {};
	virtual ~CUserEntry(){};

	const CString& GetPhoneNumber() const{ return _phone_number;} const
	void SetPhoneNumber(const CString& phone_number) { _phone_number = phone_number;}

	void SetName(const CString& name) { _name = name;}
	const CString& GetName() const{ return _name;}
		
	void ClearAll();

	void AddAlertRecord(CUserAlertRecord& alert);
	void AddSmsCommand(CCommandRecord& cmd);

	void Print() const;

	map<AddressValueKey,CUserAlertRecord>& GetEibToSmsDB() { return _eib_to_sms_db;}
	map<CString,CCommandRecord>& GetSmsToEibDB() { return _sms_to_eib_db;}

private:
	CString _name;
	CString _phone_number;
	map<AddressValueKey,CUserAlertRecord> _eib_to_sms_db;
	map<CString,CCommandRecord> _sms_to_eib_db;
};

class CSMSServerDB : public CGenericDB<CString,CUserEntry>
{
public:
	CSMSServerDB();
	virtual ~CSMSServerDB();

	virtual void OnReadParamComplete(CUserEntry& current_record, const CString& param,const CString& value);
	virtual void OnReadRecordComplete(CUserEntry& current_record);
	virtual void OnReadRecordNameComplete(CUserEntry& current_record, const CString& record_name);
	virtual void OnSaveRecordStarted(const CUserEntry& record,CString& record_name, map<CString,CString>& param_values);
	
	bool FindSmsMesaages(unsigned short d_address, unsigned short value, list<CUserAlertRecord>& result) ;
	bool FindEibMessages(const CString& sms_msg, list<CCommandRecord>& result) ;

	void InteractiveConf();

	void Print();

private:
	void ParseAlertRecord(const CString& alert_record,CUserEntry& record);
	void ParseCommandRecord(const CString& command_record,CUserEntry& record);

private:
	bool AddOrUpdateUserEntry(CUserEntry& entry);
	bool DeleteUserEntry(const CString& file_name);
};

#endif
