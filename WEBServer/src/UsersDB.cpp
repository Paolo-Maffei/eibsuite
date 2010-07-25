#include "UsersDB.h"
#include "Globals.h"
#include "WEBServer.h"
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
	if(_data.size() == 0)
	{
		throw CEIBException(ConfigFileError,"No users found in \"%s\" file.",DEFAULT_USERS_DB_FILE_NAME);
	}
	return true;
}

void CUsersDB::InteractiveConf()
{
	CString file_name(CURRENT_CONF_FOLDER);
	file_name += DEFAULT_USERS_DB_FILE_NAME;
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
			goto start;
		}
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
		break;
	case 6: return;
	case -1: goto start;
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

	return true;
}

void CUsersDB::OnReadParamComplete(CUser& current_record, const CString& param,const CString& value)
{
	if (param == USER_PASSWORD_PARAM_NAME){
		current_record.SetPassword(value);
	}
	else if (param == USER_PRIVILIGES_PARAM_NAME){
		current_record.SetPriviliges(value.ToInt());
	}
	else{
		throw CEIBException(ConfigFileError,"Unknown paramerer: %s in \"%s\" file.",param.GetBuffer(),DEFAULT_USERS_DB_FILE_NAME);
	}
}

void CUsersDB::OnReadRecordComplete(CUser& current_record)
{
	if(!AddRecord(current_record.GetName(),current_record)){
		//log error
		throw CEIBException(ConfigFileError,"Duplicate \"%s\" block. please check your .conf file",current_record.GetName().GetBuffer());
	}
	current_record.SetName(EMPTY_STRING);
	current_record.SetPassword(EMPTY_STRING);
	current_record.SetPriviliges(0); 
}

void CUsersDB::Print()
{
	map<CString,CUser>::const_iterator it;
	int i = 1;
	LOG_SCREEN("WEBServer Users list\n---------------------\n");
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
	CString priv(record.GetPriviliges());
	param_values.insert(param_values.end(), pair<CString,CString>(USER_PRIVILIGES_PARAM_NAME,priv));
	param_values.insert(param_values.end(), pair<CString,CString>(USER_PASSWORD_PARAM_NAME,record.GetPassword()));
}

bool CUsersDB::AuthenticateUser(const CString& user_name, const CString& password, CUser& user)
{
	if (!GetRecord(user_name,user)){
		return false;
	}

	if (user.GetPassword() != password){
		return false;
	}
	return true;
}

/////////////////////////////////// CUser Members ///////////////////////////////////////

CUser::CUser(): _name(EMPTY_STRING),_password(EMPTY_STRING),_priviliges(0)
{
}

//copy constructor
CUser::CUser(const CUser& user)
{
	_name = user._name;
	_password = user._password;
	_priviliges = user._priviliges;
}

CUser::~CUser()
{
}

void CUser::Reset()
{
	_name.Clear();
	_password.Clear();
	_priviliges = USER_POLICY_NONE;
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

unsigned int CUser::GetPriviliges() const
{
	return _priviliges;
}

bool CUser::IsReadPolicyAllowed()
{
	return ((USER_POLICY_READ_ACCESS & _priviliges) != 0);
}

bool CUser::IsWritePolicyAllowed()
{
	return ((USER_POLICY_WRITE_ACCESS & _priviliges) != 0);
}
