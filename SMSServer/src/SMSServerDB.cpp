#include "SMSServerDB.h"
#include "HttpParser.h"

CSMSServerDB::CSMSServerDB()
{
}

CSMSServerDB::~CSMSServerDB()
{
}

void CSMSServerDB::OnReadParamComplete(CUserEntry& current_record, const CString& param,const CString& value)
{
	if(param == ALERT_PARAM_STR){
		ParseAlertRecord(value,current_record);
	}
	else if(param == SMS_COMMAND_STR){
		ParseCommandRecord(value,current_record);
	}
}

void CSMSServerDB::OnReadRecordComplete(CUserEntry& current_record)
{
	this->AddRecord(current_record.GetName(),current_record);
}

void CSMSServerDB::OnReadRecordNameComplete(CUserEntry& current_record, const CString& record_name)
{
	CString prefix(PHONE_NUMBER_BLOCK_PREFIX_STR);
	if(record_name.Find(prefix) == -1) return;
	
	current_record.ClearAll();
	current_record.SetName(record_name);
	current_record.SetPhoneNumber(record_name.SubString(prefix.GetLength(), record_name.GetLength() - prefix.GetLength()));
}

void CSMSServerDB::OnSaveRecordStarted(const CUserEntry& record,CString& record_name, map<CString,CString>& param_values)
{
	return;
}

void CSMSServerDB::ParseAlertRecord(const CString& alert_record,CUserEntry& record)
{
	StringTokenizer tok(alert_record,ALERT_PARAM_SEPERATOR_STR);

	if (tok.CountTokens() != 3){
		throw CEIBException(ConfigFileError,"error in db file. \"%s\" entry is incorrect",alert_record.GetBuffer());
	}

	CUserAlertRecord alert;
	
	unsigned short d_address;
	if(!tok.NextToken().UShortFromHexString(d_address)){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",alert_record.GetBuffer());
	}

	unsigned char value;
	if(!tok.NextToken().UCharFromHexString(value)){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",alert_record.GetBuffer());
	}

	alert.SetDestAddress(d_address);
	alert.SetValue(value);
	alert.SetTextMessage(tok.NextToken());
	alert.SetPhoneNumber(record.GetPhoneNumber());

	record.AddAlertRecord(alert);
}

void CSMSServerDB::ParseCommandRecord(const CString& command_record,CUserEntry& record)
{
	StringTokenizer tok(command_record,CMD_PARAM_SEPERATO_STR);

	if (tok.CountTokens() != 3){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",command_record.GetBuffer());
	}

	CCommandRecord cmd;

	unsigned short d_address;
	if(!tok.NextToken().UShortFromHexString(d_address)){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",command_record.GetBuffer());
	}

	unsigned char value;
	if(!tok.NextToken().UCharFromHexString(value)){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",command_record.GetBuffer());
	}

	cmd.SetDestAddress(d_address);
	cmd.SetValue(value);
	cmd.SetTextMessage(tok.NextToken());
	cmd.SetPhoneNumber(record.GetPhoneNumber());

	record.AddSmsCommand(cmd);
}

bool CSMSServerDB::FindSmsMesaages(unsigned short d_address, unsigned short value, list<CUserAlertRecord>& result)
{
	//first clear the list
	result.clear();

	//now find
	map<CString,CUserEntry>::iterator it;
	for(it = _data.begin();it != _data.end();++it)
	{
		CUserEntry& tmp = it->second;
		map<AddressValueKey,CUserAlertRecord>::iterator sec_it;
		AddressValueKey avk(d_address,value);
		sec_it = tmp.GetEibToSmsDB().find(avk);
		if (sec_it != tmp.GetEibToSmsDB().end()){
			result.push_back(sec_it->second);
		}
	}

	return (result.size() > 0);
}

bool CSMSServerDB::FindEibMessages(const CString& sms_msg, list<CCommandRecord>& result) 
{
	//first clear the list
	result.clear();
	//now find
	map<CString,CUserEntry>::iterator it;
	for(it = _data.begin();it != _data.end();++it)
	{
		CUserEntry& tmp = it->second;
		map<CString,CCommandRecord>::iterator sec_it;
		sec_it = tmp.GetSmsToEibDB().find(sms_msg);
		if (sec_it != tmp.GetSmsToEibDB().end()){
			result.push_back(sec_it->second);
		}
	}

	return (result.size() > 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CUserEntry::AddAlertRecord(CUserAlertRecord& alert)
{
	unsigned short d_address = alert.GetDestAddress();
	unsigned short value = alert.GetValue();
	GetEibToSmsDB().insert(pair<AddressValueKey,CUserAlertRecord>(AddressValueKey(d_address,value),alert));
}

void CUserEntry::AddSmsCommand(CCommandRecord& cmd)
{
	GetSmsToEibDB().insert(pair<CString,CCommandRecord>(cmd.GetTextMessage(),cmd));
}


void CUserEntry::ClearAll()
{
	_name = EMPTY_STRING;
	_text_msg = EMPTY_STRING;
	_phone_number = EMPTY_STRING;
	_eib_to_sms_db.clear();
	_sms_to_eib_db.clear();
}