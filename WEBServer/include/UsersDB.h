#ifndef __USERS_DATA_BASE_HEADER__
#define __USERS_DATA_BASE_HEADER__

#include "CString.h"
#include <map>
#include <fstream>
#include "GenericDB.h"
#include "LogFile.h"

#define USER_POLICY_NONE			0x0
#define USER_POLICY_READ_ACCESS		0x1
#define USER_POLICY_WRITE_ACCESS	0x2

#define USER_PASSWORD_PARAM_NAME "PASSWORD"
#define USER_PRIVILIGES_PARAM_NAME "PRIVILIGES"

class CUsersDB;

class CUser 
{
public:
	CUser();
	CUser(const CUser& user);
	virtual ~CUser();

	bool IsReadPolicyAllowed();
	bool IsWritePolicyAllowed();

	const CString& GetName() const;
	const CString& GetPassword() const;
	unsigned int GetPriviliges() const;

	void Print() const;

	void SetName(const CString& name) { _name = name; }
	void SetPassword(const CString& password) { _password = password;}
	void SetPriviliges(int priviliges) { _priviliges = priviliges;}

	friend class CUsersDB;

private:
	void Reset();

private:
	CString _name;
	CString _password;
	unsigned int _priviliges;
};

class CUsersDB : public CGenericDB<CString,CUser>
{
public:

	CUsersDB();
	virtual ~CUsersDB();

	virtual void Init(const CString& file_name);
	virtual void Print();
	const map<CString,CUser>& GetUsersList() const { return _data;}
	bool AuthenticateUser(const CString& user_name, const CString& password, CUser& user);
	int GetNumOfUsers() const { return _data.size(); }
	bool Validate();

	void InteractiveConf();

	virtual void OnReadParamComplete(CUser& current_record, const CString& param,const CString& value);
	virtual void OnReadRecordComplete(CUser& current_record);
	virtual void OnReadRecordNameComplete(CUser& current_record, const CString& record_name);
	virtual void OnSaveRecordStarted(const CUser& record,CString& record_name, list<pair<CString,CString> >& param_values);

private:
	bool AddOrUpdateUser(CUser& user);
	bool UpdateUser(const CString& file_name);
	bool DeleteUser(const CString& file_name);
};

#endif
