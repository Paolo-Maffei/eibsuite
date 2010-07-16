#include "UsersDB.h"
#include "Globals.h"
#include "EIBServer.h"

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
	unsigned int priv = 0;
	if(record.IsReadPolicyAllowed())
		priv |= USER_POLICY_READ_ACCESS;
	if(record.IsWritePolicyAllowed())
		priv |= USER_POLICY_WRITE_ACCESS;
	if(record.IsConsoleAccessAllowed())
		priv |= USER_POLICY_CONSOLE_ACCESS;

	CString temp(priv);
	param_values.insert(pair<CString,CString>(USER_PRIVILIGES_PARAM_NAME,temp));
	param_values.insert(pair<CString,CString>(USER_PASSWORD_PARAM_NAME,record.GetPassword()));
	param_values.insert(pair<CString,CString>(USER_ALLOWED_SOURCE_MASK,CString::ToHexFormat(record.GetFilter().GetSrcMask())));
	param_values.insert(pair<CString,CString>(USER_ALLOWED_DEST_MASK,CString::ToHexFormat(record.GetFilter().GetDstMask())));
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
	_filter = user._filter;
}

CUser::~CUser()
{
}

void CUser::Print() const
{
	cout << "User Name : " << _name.GetBuffer() << endl;
	cout << "Password : " << _password.GetBuffer() << endl;
	cout << "Privileges : " << _priviliges << endl;
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
