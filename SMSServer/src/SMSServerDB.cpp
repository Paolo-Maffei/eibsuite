#include "SMSServerDB.h"
#include "HttpParser.h"
#include "cli.h"
#include "SMSServer.h"

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
	record_name = record.GetName();
	const map<AddressValueKey,CUserAlertRecord>& m1 = record.GetEibToSmsDBConst();
	map<AddressValueKey,CUserAlertRecord>::const_iterator it1 = m1.begin();

	char val[512];
	for(; it1 != m1.end(); it1++)
	{
		sprintf(val, "%s:0x%x:%s", CEibAddress(it1->second.GetDestAddress(), true).ToString().GetBuffer(), it1->second.GetValue(), it1->second.GetTextMessage().GetBuffer());
		param_values.insert(pair<CString,CString>(ALERT_PARAM_STR,val));
	}

	const map<CString,CUserAlertRecord>& m2 = record.GetSmsToEibDBConst();
	map<CString,CUserAlertRecord>::const_iterator it2 = m2.begin();

	for(; it2 != m2.end(); it2++)
	{
		sprintf(val, "%s:0x%x:%s", it2->second.GetTextMessage().GetBuffer(), it2->second.GetValue(), CEibAddress(it2->second.GetDestAddress(), true).ToString().GetBuffer());
		param_values.insert(pair<CString,CString>(SMS_COMMAND_STR,val));
	}

	return;
}

void CSMSServerDB::ParseAlertRecord(const CString& alert_record,CUserEntry& record)
{
	StringTokenizer tok(alert_record,ALERT_PARAM_SEPERATOR_STR);

	if (tok.CountTokens() != 3){
		throw CEIBException(ConfigFileError,"error in db file. \"%s\" entry is incorrect",alert_record.GetBuffer());
	}

	CUserAlertRecord alert;
	
	START_TRY
		CEibAddress addr(tok.NextToken());
		alert.SetDestAddress(addr.ToByteArray());
	END_TRY_START_CATCH(e)
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect: %s",alert_record.GetBuffer(), e.what());
	END_CATCH

	unsigned char value;
	if(!tok.NextToken().UCharFromHexString(value)){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",alert_record.GetBuffer());
	}

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

	CUserAlertRecord cmd;

	cmd.SetTextMessage(tok.NextToken());

	unsigned char value;
	if(!tok.NextToken().UCharFromHexString(value)){
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect",command_record.GetBuffer());
	}

	START_TRY
		CEibAddress addr(tok.NextToken());
		cmd.SetDestAddress(addr.ToByteArray());
	END_TRY_START_CATCH(e)
		throw CEIBException(ConfigFileError,"error in SMS db file. \"%s\" entry is incorrect: %s",command_record.GetBuffer(), e.what());
	END_CATCH

	cmd.SetValue(value);

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

bool CSMSServerDB::FindEibMessages(const CString& sms_msg, list<CUserAlertRecord>& result)
{
	//first clear the list
	result.clear();
	//now find
	map<CString,CUserEntry>::iterator it;
	for(it = _data.begin();it != _data.end();++it)
	{
		CUserEntry& tmp = it->second;
		map<CString,CUserAlertRecord>::iterator sec_it;
		sec_it = tmp.GetSmsToEibDB().find(sms_msg);
		if (sec_it != tmp.GetSmsToEibDB().end()){
			result.push_back(sec_it->second);
		}
	}

	return (result.size() > 0);
}

void CSMSServerDB::InteractiveConf()
{
	CString file_name(CURRENT_CONF_FOLDER);
	file_name += SMS_DB_FILE;
	Init(file_name);
	Load();


start:
	LOG_SCREEN("************************************************\n");
	LOG_SCREEN("SMS Messages Mappings Interactive configuration:\n");
	LOG_SCREEN("************************************************\n");

	CUserEntry entry;

	int ival;
	map<int,CString> map1;
	map1.insert(map1.end(),pair<int,CString>(1,"Add Phone Number"));
	map1.insert(map1.end(),pair<int,CString>(2,"Delete Phone Number"));
	map1.insert(map1.end(),pair<int,CString>(3,"Edit Phone Number Mappings"));
	map1.insert(map1.end(),pair<int,CString>(4,"Print Current entries"));
	map1.insert(map1.end(),pair<int,CString>(5,"Save & Quit"));
	map1.insert(map1.end(),pair<int,CString>(6,"Quit without saving"));
	ConsoleCLI::GetStrOption("Choose one of the above options:", map1, ival, NO_DEFAULT_OPTION);

	switch (ival)
	{
	case 1:
		entry.ClearAll();
		if(AddUserEntry(entry) && AddRecord(entry.GetName(), entry)){
			LOG_SCREEN("Phone entry Added successfully.\n");
		}
		break;
	case 2:
		if(DeleteUserEntry(file_name)){
			LOG_SCREEN("Phone entry Deleted successfully.\n");
		}
		break;
	case 3:
		if(EditUserEntry(file_name)){
			LOG_SCREEN("Phone entry Updated successfully.\n");
		}
		break;
	case 4:
		if(_data.size() == 0)
			LOG_SCREEN("No Phone number entries found in in file \"%s\"\n", file_name.GetBuffer());
		else
			Print();
		break;
	case 5:
		Save();
	case 6:
		return;
	case NO_DEFAULT_OPTION:  //when user press just enter...
		break;
	default:
		LOG_SCREEN("Unknown Option\n");
		break;
	}
	goto start;
}

bool CSMSServerDB::AddUserEntry(CUserEntry& entry)
{
	CString sval;
	if(!ConsoleCLI::GetCString("Enter Phone number: ",sval, entry.GetName())){
		return false;
	}
	entry.SetPhoneNumber(sval);
	CString name(PHONE_NUMBER_BLOCK_PREFIX_STR);
	entry.SetName(name + sval);
	return true;
}

bool CSMSServerDB::EditUserEntry(const CString& file_name)
{
	if(_data.size() == 0){
		LOG_SCREEN("No Phone number entries found in in file \"%s\"\n", file_name.GetBuffer());
		return false;
	}
	int i = 1, ival;
	map<CString,CUserEntry>::iterator it;
	Print();
	if(!ConsoleCLI::GetIntRange("Select a Phone number to Edit: ",ival, 1, _data.size(), NO_DEFAULT_OPTION)){
		return false;
	}
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		if (i++ != ival){
			continue;
		}else{
			//now we can start editing
			return it->second.Edit();
		}
	}
	return false;
}

bool CSMSServerDB::DeleteUserEntry(const CString& file_name)
{
	if(_data.size() == 0){
		LOG_SCREEN("No Phone number entries found in in file \"%s\"\n", file_name.GetBuffer());
		return false;
	}
	int i = 1, ival;
	map<CString,CUserEntry>::const_iterator it;
	Print();
	if(!ConsoleCLI::GetIntRange("Select a Phone number to delete: ",ival, 1, _data.size(), NO_DEFAULT_OPTION)){
		return false;
	}
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		if (i++ != ival){
			continue;
		}else{
			return DeleteRecord(it->second.GetName());
		}
	}
	return false;
}

void CSMSServerDB::Print()
{
	map<CString,CUserEntry>::const_iterator it;
	int i = 1;
	LOG_SCREEN("Phone numbers list\n---------------------\n");
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		LOG_SCREEN("%d. ", i++);
		it->second.Print();
		LOG_SCREEN("\n");
	}
	LOG_SCREEN("\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CUserEntry::AddAlertRecord(CUserAlertRecord& alert)
{
	unsigned short d_address = alert.GetDestAddress();
	unsigned short value = alert.GetValue();
	return GetEibToSmsDB().insert(pair<AddressValueKey,CUserAlertRecord>(AddressValueKey(d_address,value),alert)).second;
}

bool CUserEntry::AddSmsCommand(CUserAlertRecord& cmd)
{
	return GetSmsToEibDB().insert(pair<CString,CUserAlertRecord>(cmd.GetTextMessage(),cmd)).second;
}


void CUserEntry::ClearAll()
{
	_name.Clear();
	_phone_number.Clear();
	_eib_to_sms_db.clear();
	_sms_to_eib_db.clear();
}

void CUserEntry::Print() const
{
	LOG_SCREEN("%s", _name.GetBuffer());
}

void CUserEntry::PrintAllMappings()
{
	LOG_SCREEN("%s Messages Mappings:\n", _name.GetBuffer());
	LOG_SCREEN("-------------------------\n");

	map<AddressValueKey,CUserAlertRecord>::const_iterator it1;
	int i = 1;
	for( it1 = _eib_to_sms_db.begin(); it1 != _eib_to_sms_db.end(); ++it1){
		LOG_SCREEN("%d. [EIB --> SMS] %s:0x%x:%s\n", i++,
													   CEibAddress(it1->second.GetDestAddress(), true).ToString().GetBuffer(),
													   it1->second.GetValue(),
													   it1->second.GetTextMessage().GetBuffer());
	}
	map<CString,CUserAlertRecord>::const_iterator it2;
	for( it2 = _sms_to_eib_db.begin(); it2 != _sms_to_eib_db.end(); ++it2){
		LOG_SCREEN("%d. [SMS --> EIB] %s:0x%x:%s\n", i++,
													it2->second.GetTextMessage().GetBuffer(),
													it2->second.GetValue(),
													CEibAddress(it2->second.GetDestAddress(), true).ToString().GetBuffer());
	}
}

bool CUserEntry::Edit()
{
start:
	LOG_SCREEN("\nEdit Entry [%s]:\n",_name.GetBuffer());
	LOG_SCREEN("-----------------------------\n");

	CUserEntry entry;

	int ival;
	map<int,CString> map1;
	map1.insert(map1.end(),pair<int,CString>(1,"Add EIB --> SMS mapping entry"));
	map1.insert(map1.end(),pair<int,CString>(2,"Add SMS --> EIB mapping entry"));
	map1.insert(map1.end(),pair<int,CString>(3,"Delete mapping entry"));
	map1.insert(map1.end(),pair<int,CString>(4,"Print all mappings"));
	map1.insert(map1.end(),pair<int,CString>(5,"Quit"));
	ConsoleCLI::GetStrOption("Choose one of the above options:", map1, ival, NO_DEFAULT_OPTION);

	bool dirty = false;

	switch (ival)
	{
	case 1:
		if(AddSingleMapping(true, false)){
			LOG_SCREEN("\nEntry added successfully.\n");
			dirty = true;
		}
		break;
	case 2:
		if(AddSingleMapping(false, true)){
			LOG_SCREEN("\nEntry added successfully.\n");
			dirty = true;
		}
		break;
	case 3:
		if(DeleteSingleMapping()){
			LOG_SCREEN("Mapping entry deleted successfully\n.");
			dirty = true;
		}
		break;
	case 4:
		if(_eib_to_sms_db.size() == 0 && _sms_to_eib_db.size() == 0){
			LOG_SCREEN("No Messages mapping entries found for \"%s\"\n", _name.GetBuffer());
			break;
		}
		PrintAllMappings();
		break;
	case 5: return dirty;
		break;
	case NO_DEFAULT_OPTION:
		break;
	default:
		break;
	}
	goto start;
}

bool CUserEntry::DeleteSingleMapping()
{
	if(_eib_to_sms_db.size() == 0 && _sms_to_eib_db.size() == 0){
		LOG_SCREEN("No Messages mapping entries found for \"%s\"\n", _name.GetBuffer());
		return false;
	}
	PrintAllMappings();
	int i = 1, ival;
	if(!ConsoleCLI::GetIntRange("Select a mapping entry to delete: ",ival, 1, _eib_to_sms_db.size() +  _sms_to_eib_db.size(), NO_DEFAULT_OPTION)){
		return false;
	}

	if(ival <= (int)_eib_to_sms_db.size()){
		map<AddressValueKey,CUserAlertRecord>::iterator it;
		for( it = _eib_to_sms_db.begin(); it != _eib_to_sms_db.end(); ++it)
		{
			if (i++ != ival){
				continue;
			}else{
				_eib_to_sms_db.erase(it);
				return true;
			}
		}
	}
	else{
		map<CString,CUserAlertRecord>::iterator it;
		for( it = _sms_to_eib_db.begin(); it != _sms_to_eib_db.end(); ++it)
		{
			if (i++ != ival){
				continue;
			}else{
				_sms_to_eib_db.erase(it);
				return true;
			}
		}
	}

	return false;
}

bool CUserEntry::AddSingleMapping(bool eib2sms, bool sms2eib)
{
	CUserAlertRecord record;

	CString sval;
	if(!ConsoleCLI::GetCString("Enter EIB Destination address: ",sval, "1/1/1")){
		return false;
	}
	START_TRY
		CEibAddress addr(sval);
		record.SetDestAddress(addr.ToByteArray());
	END_TRY_START_CATCH(e)
		LOG_SCREEN("Error: Illegal EIB Address.");
		return false;
	END_CATCH

	if(!ConsoleCLI::GetHexString("Enter APCI value: ", sval, "0x80", true)){
		return false;
	}
	unsigned char t;
	if(sval.UCharFromHexString(t)){
		record.SetValue(t);
	}else{
		LOG_SCREEN("ERROR: Cannot convert HEX string value.");
		return false;
	}


	if(!ConsoleCLI::GetCString("Enter SMS Text message: ",sval, EMPTY_STRING)){
		return false;
	}
	record.SetTextMessage(sval);

	if(eib2sms && !sms2eib){
		return AddAlertRecord(record);
	}else if(!eib2sms && sms2eib){
		return AddSmsCommand(record);
	}

	return false;
}
