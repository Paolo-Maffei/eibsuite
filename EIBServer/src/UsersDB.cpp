#include "UsersDB.h"
#include "Globals.h"
#include "EIBServer.h"
#include "cli.h"

CUsersDB::CUsersDB() 
{
}

CUsersDB::~CUsersDB()
{
}

void CUsersDB::Init(const CString& file_name)
{
	CGenericDB<CString,CUser>::Init(file_name);
}

bool CUsersDB::Validate()
{
	if(IsEmpty())
	{
		throw CEIBException(ConfigFileError,"No users found in \"%s\" file.",DEFAULT_USERS_DB_FILE);
	}

	return true;
}

void CUsersDB::OnReadParamComplete(CUser& current_record, const CString& param,const CString& value)
{
	if (param == USER_PASSWORD_PARAM_NAME){
		current_record.SetPassword(value);
	}
	else if (param == USER_PRIVILIGES_PARAM_NAME){
		current_record.SetPriviliges(value.ToUInt());
	}
	/*
	else if(param == USER_ALLOWED_SOURCE_ADDRESS){
		unsigned short address;
		if(value.UShortFromHexString(address)) current_record.GetFilter().AddAllowedSourceAddress(address);
	}
	*/
	else if(param == USER_ALLOWED_SOURCE_MASK){
		unsigned short mask;
		if(value.UShortFromHexString(mask)) current_record.SetAllowedSourceAddressMask(mask);
	}
	/*
	else if(param == USER_ALLOWED_DEST_ADDRESS){
		unsigned short address;
		if(value.UShortFromHexString(address)) current_record.GetFilter().AddAllowedDestAddress(address);
	}
	*/
	else if(param == USER_ALLOWED_DEST_MASK){
		unsigned short mask;
		if(value.UShortFromHexString(mask)) current_record.SetAllowedDestAddressMask(mask);
	}
}

void CUsersDB::OnReadRecordComplete(CUser& current_record)
{
	if(!AddRecord(current_record.GetName(),current_record)){
		//log error
		throw CEIBException(ConfigFileError,"Duplicate user name block. please check your .conf file");
	}
	current_record.SetName(EMPTY_STRING);
	current_record.SetPassword(EMPTY_STRING);
	current_record.SetPriviliges(0); 
}

void CUsersDB::Print() const
{
	map<CString,CUser>::const_iterator it;
	int i = 1;
	LOG_SCREEN("EIBServer Users list\n---------------------\n");
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		LOG_SCREEN("%d. ", i++);
		it->second.Print();
		LOG_SCREEN("\n");
	}
	LOG_SCREEN("\n");
}

void CUsersDB::OnReadRecordNameComplete(CUser& current_record, const CString& record_name)
{
	current_record.SetName(record_name);
}

void CUsersDB::OnSaveRecordStarted(const CUser& record,CString& record_name, list<pair<CString,CString> >& param_values)
{
	record_name = record.GetName();
	unsigned int priv = 0;
	if(record.IsReadPolicyAllowed())
		priv |= USER_POLICY_READ_ACCESS;
	if(record.IsWritePolicyAllowed())
		priv |= USER_POLICY_WRITE_ACCESS;
	if(record.IsConsoleAccessAllowed())
		priv |= USER_POLICY_CONSOLE_ACCESS;

	CString temp(priv);
	param_values.insert(param_values.end(), pair<CString,CString>(USER_PRIVILIGES_PARAM_NAME,temp));
	param_values.insert(param_values.end(), pair<CString,CString>(USER_PASSWORD_PARAM_NAME,record.GetPassword()));
	param_values.insert(param_values.end(), pair<CString,CString>(USER_ALLOWED_SOURCE_MASK,CString::ToHexFormat(record.GetFilter().GetSrcMask())));
	param_values.insert(param_values.end(), pair<CString,CString>(USER_ALLOWED_DEST_MASK,CString::ToHexFormat(record.GetFilter().GetDstMask())));
}

void CUsersDB::InteractiveConf()
{
	CString file_name(CURRENT_CONF_FOLDER);
	file_name += DEFAULT_USERS_DB_FILE;
	Init(file_name);
	Load();

	CUser user;

start:
	LOG_SCREEN("************************************\n");
	LOG_SCREEN("Users DB Interactive configuration:\n");
	LOG_SCREEN("************************************\n");


	int ival;
	map<int,CString> map1;
	map1.insert(map1.end(),pair<int,CString>(1,"Add User"));
	map1.insert(map1.end(),pair<int,CString>(2,"Delete User"));
	map1.insert(map1.end(),pair<int,CString>(3,"Update User"));
	map1.insert(map1.end(),pair<int,CString>(4,"Print Users List"));
	map1.insert(map1.end(),pair<int,CString>(5,"Save & Quit"));
	map1.insert(map1.end(),pair<int,CString>(6,"Quit without saving"));
	ConsoleCLI::GetStrOption("Choose one of the above options:", map1, ival, NO_DEFAULT_OPTION);
	switch (ival)
	{
	case 1:
		user.Reset();
		if(AddOrUpdateUser(user) && AddRecord(user.GetName(), user)){
			LOG_SCREEN("User Added successfully.\n");
			goto start;
		}
		break;
	case 2:
		if(DeleteUser(file_name)){
			LOG_SCREEN("User Deleted successfully.\n");
		}
		goto start;
		break;
	case 3:
		if(UpdateUser(file_name)){
			LOG_SCREEN("User Updated successfully.\n");
			goto start;
		}
		break;
	case 4:
		if(_data.size() == 0)
			LOG_SCREEN("No Users found in in file \"%s\"\n", file_name.GetBuffer());
		else
			Print();
		goto start;
		break;
	case 5: Save();
		CEIBServer::GetInstance().GetLog().SetConsoleColor(GREEN);
		LOG_INFO("\nUsers file: \"%s\" saved successfully. the new file will be loaded automatically.\n", file_name.GetBuffer());
		CEIBServer::GetInstance().GetLog().SetConsoleColor(WHITE);
		LOG_SCREEN("\n\n");
		break;
	case 6: return;
	case NO_DEFAULT_OPTION: goto start;
	default:
		LOG_SCREEN("Unknown Option\n");
		goto start;
	}
}
bool CUsersDB::UpdateUser(const CString& file_name)
{
	if(_data.size() == 0){
		LOG_SCREEN("No Users found in in file \"%s\"\n", file_name.GetBuffer());
		return false;
	}
	int i = 1, ival;
	map<CString,CUser>::iterator it;
	Print();
	if(!ConsoleCLI::GetIntRange("Select a user to Update: ",ival, 1, _data.size(), NO_DEFAULT_OPTION)){
		return false;
	}
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		if (i++ != ival){
			continue;
		}else{
			return AddOrUpdateUser(it->second);
		}
	}
	return false;
}

bool CUsersDB::DeleteUser(const CString& file_name)
{
	if(_data.size() == 0){
		LOG_SCREEN("No Users found in in file \"%s\"\n", file_name.GetBuffer());
		return false;
	}
	int i = 1, ival;
	map<CString,CUser>::const_iterator it;
	Print();
	if(!ConsoleCLI::GetIntRange("Select a user to delete: ",ival, 1, _data.size(), NO_DEFAULT_OPTION)){
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

bool CUsersDB::AddOrUpdateUser(CUser& user)
{
	CString sval;
	int ival;
	bool bval;

	if(!ConsoleCLI::GetCString("Enter User name: ",sval, user.GetName())){
		return false;
	}
	user.SetName(sval);
	if(!ConsoleCLI::GetCString("Enter User password: ",sval, user.GetPassword())){
		return false;
	}
	user.SetPassword(sval);

	map<int,CString> map1;
	map1.insert(map1.end(),pair<int,CString>(1,"None"));
	map1.insert(map1.end(),pair<int,CString>(2,"Read only"));
	map1.insert(map1.end(),pair<int,CString>(3,"Write only"));
	map1.insert(map1.end(),pair<int,CString>(4,"Read & Write"));

	int temp = 1;
	if(user.IsReadPolicyAllowed()){
		temp = user.IsWritePolicyAllowed() ? 4 : 2;
	}else if(user.IsWritePolicyAllowed()){
		temp = 3;
	}

	ConsoleCLI::GetStrOption("Set user Privileges:", map1, ival, temp);
	switch (ival)
	{
	case 1: user.SetPriviliges(USER_POLICY_NONE);
		break;
	case 2: user.SetPriviliges(USER_POLICY_READ_ACCESS);
		break;
	case 3: user.SetPriviliges(USER_POLICY_WRITE_ACCESS);
		break;
	case 4: user.SetPriviliges(USER_POLICY_READ_ACCESS | USER_POLICY_WRITE_ACCESS);
		break;
	default: LOG_SCREEN("Unknown option. canceling...\n");
		return false;
	}

	if(ConsoleCLI::Getbool("Allow this user to connect through EIBConsole?",bval, user.IsConsoleAccessAllowed())){
		if(bval){
			user.SetPriviliges(user._priviliges | USER_POLICY_CONSOLE_ACCESS);
		}else{
			user.SetPriviliges(user._priviliges & ~USER_POLICY_CONSOLE_ACCESS);
		}
	}

	return true;
}

/////////////////////////////////// CUser Members ///////////////////////////////////////

CUser::CUser(): _name(EMPTY_STRING),_password(EMPTY_STRING),_priviliges(USER_POLICY_NONE)
{
}

//copy constructor
CUser::CUser(const CUser& user)
{
	_name = user._name;
	_password = user._password;
	_priviliges = user._priviliges;
	_filter = user._filter;
}

CUser::~CUser()
{
}

void CUser::Reset()
{
	_priviliges = USER_POLICY_NONE;
	_name.Clear();
	_password.Clear();
}

void CUser::Print() const
{
	LOG_SCREEN("Name: %s, Password: %s, Priviliges: %d", _name.GetBuffer(), _password.GetBuffer(), _priviliges);
}

const CString& CUser::GetName() const
{
	return _name;
}

const CString& CUser::GetPassword() const
{
	return _password;
}

bool CUser::IsReadPolicyAllowed() const
{
	return ((USER_POLICY_READ_ACCESS & _priviliges) != 0);
}

bool CUser::IsWritePolicyAllowed() const
{
	return ((USER_POLICY_WRITE_ACCESS & _priviliges) != 0);
}
