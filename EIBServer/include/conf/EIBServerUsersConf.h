#ifndef __CONNECTED_CLIENTS_CONF_HEADER__
#define __CONNECTED_CLIENTS_CONF_HEADER__

#include "IConfBase.h"
#include <list>

//defs
#define EIB_SERVER_USERS_LIST_XML "EIB_SERVER_USERS_LIST"
#define EIB_SERVER_USER_XML "EIB_SERVER_USER"
#define EIB_SERVER_USER_IP_ADDRESS_XML "EIB_SERVER_USER_IP_ADDRESS"
#define EIB_SERVER_USER_NAME_XML "EIB_SERVER_USER_NAME"
#define EIB_SERVER_USER_PASSWORD_XML "EIB_SERVER_USER_PASSWORD"
#define EIB_SERVER_USER_IS_CONNECTED_XML "EIB_SERVER_USER_IS_CONNECTED"
#define EIB_SERVER_USER_SESSION_ID_XML "EIB_SERVER_USER_SESSION_ID"
#define EIB_SERVER_USER_PRIVILIGES_XML "EIB_SERVER_USER_PRIVILIGES"
#define EIB_SERVER_USER_SOURCE_ADDR_MASK_XML "EIB_SERVER_USER_SOURCE_ADDR_MASK"
#define EIB_SERVER_USER_DST_ADDR_MASK_XML "EIB_SERVER_USER_DST_ADDR_MASK"

class CClientConf;

class CEIBServerUsersConf : public IConfBase
{
public:
	CEIBServerUsersConf();
	virtual ~CEIBServerUsersConf();

	virtual void ToXml(CDataBuffer& xml_str);
	virtual void FromXml(const CDataBuffer& xml_str);

	void GetConnectedClients();
	void SetConnectedClients();

private:
	list<CClientConf> _clients;
};

class CClientConf
{
public:
	CClientConf(){};
	virtual ~CClientConf(){};

	friend class CEIBServerUsersConf;
private:
	CString _name;
	CString _password;
	CString _ip_address;
	bool _connected;
	int _session_id;
	int _priviliges;
	unsigned short _sa_mask;
	unsigned short _da_mask;
	bool _is_current_user;
};

#endif
