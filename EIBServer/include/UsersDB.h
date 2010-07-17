#ifndef __USERS_DATA_BASE_HEADER__
#define __USERS_DATA_BASE_HEADER__

#include "CString.h"
#include <map>
#include <fstream>
#include "GenericDB.h"
#include "EibNetwork.h"
#include "LogFile.h"
#include "PacketFilter.h"

#define USER_POLICY_NONE			0x0
#define USER_POLICY_READ_ACCESS		0x1
#define USER_POLICY_WRITE_ACCESS	0x2
#define USER_POLICY_CONSOLE_ACCESS	0x4

#define USER_PASSWORD_PARAM_NAME "PASSWORD"
#define USER_PRIVILIGES_PARAM_NAME "PRIVILIGES"
//Allowed
#define USER_ALLOWED_SOURCE_ADDRESS "ALLOWED_SOURCE_ADDRESS"
#define USER_ALLOWED_SOURCE_MASK "ALLOWED_SOURCE_MASK"
#define USER_ALLOWED_DEST_ADDRESS "ALLOWED_DEST_ADDRESS"
#define USER_ALLOWED_DEST_MASK "ALLOWED_DEST_MASK"

class CEIBServer;
class CUsersDB;

class CUser 
{
public:
	CUser();
	CUser(const CUser& user);
	virtual ~CUser();

	bool IsReadPolicyAllowed() const;
	bool IsWritePolicyAllowed() const;
	bool IsConsoleAccessAllowed() const { return (_priviliges & USER_POLICY_CONSOLE_ACCESS ) != 0; }

	const CString& GetName() const;
	const CString& GetPassword() const;
	unsigned short GetSrcMask() const { return _filter.GetSrcMask(); }
	unsigned short GetDstMask() const { return _filter.GetDstMask(); }

	void Print() const;

	void SetName(const CString& name) { _name = name; }
	void SetPassword(const CString& password) { _password = password;}
	void SetPriviliges(int priviliges) { _priviliges = priviliges;}

	void SetAllowedSourceAddressMask(unsigned short mask) { _filter.SetAllowedSourceAddressMask(mask);}
	void SetAllowedDestAddressMask(unsigned short mask) { _filter.SetAllowedDestAddressMask(mask);}

	const CPacketFilter& GetFilter() const { return _filter;}

	friend class CUsersDB;

private:
	void Reset();

private:
	CString _name;
	CString _password;
	unsigned int _priviliges;
	CPacketFilter _filter;
};

class CUsersDB : public CGenericDB<CString,CUser>
{
public:

	CUsersDB();
	virtual ~CUsersDB();

	virtual void Init(const CString& file_name);
	virtual void Print() const;
	const map<CString,CUser>& GetUsersList() const { return _data;}
	
	bool Validate();

	void InteractiveConf();

	virtual void OnReadParamComplete(CUser& current_record, const CString& param,const CString& value);
	virtual void OnReadRecordComplete(CUser& current_record);
	virtual void OnReadRecordNameComplete(CUser& current_record, const CString& record_name);
	virtual void OnSaveRecordStarted(const CUser& record,CString& record_name, map<CString,CString>& param_values);

private:
	bool AddOrUpdateUser(CUser& user);
	bool DeleteUser(const CString& file_name);
	bool UpdateUser(const CString& file_name);
};

#endif
