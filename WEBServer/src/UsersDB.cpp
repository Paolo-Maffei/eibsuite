#include "UsersDB.h"
#include "Globals.h"
#include "WEBServer.h"

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
	for( it = _data.begin(); it != _data.end(); ++it)
	{
		it->second.Print();
		cout << "*****************"<< endl;
	}
}

void CUsersDB::OnReadRecordNameComplete(CUser& current_record, const CString& record_name)
{
	current_record.SetName(record_name);
}

void CUsersDB::OnSaveRecordStarted(const CUser& record,CString& record_name, map<CString,CString>& param_values)
{
	record_name = record.GetName();
	CString priv(record.GetPriviliges());
	param_values.insert(pair<CString,CString>(USER_PRIVILIGES_PARAM_NAME,priv));
	param_values.insert(pair<CString,CString>(USER_PASSWORD_PARAM_NAME,record.GetPassword()));
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

void CUser::Print() const
{
	cout << "User Name : " << _name.GetBuffer() << endl;
	cout << "Password : " << _password.GetBuffer() << endl;
	cout << "Priviliges : " << _priviliges << endl;
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
