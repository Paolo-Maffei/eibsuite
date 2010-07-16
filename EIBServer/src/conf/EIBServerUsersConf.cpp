#include "conf/EIBServerUsersConf.h"
#include "EIBServer.h"

CEIBServerUsersConf::CEIBServerUsersConf()
{
}

CEIBServerUsersConf::~CEIBServerUsersConf()
{
}

void CEIBServerUsersConf::ToXml(CDataBuffer& xml_str)
{
	list<CClientConf>::iterator it;
	
	CXmlElement users_list = _doc.RootElement().InsertChild(EIB_SERVER_USERS_LIST_XML);
	
	for(it = _clients.begin();it!= _clients.end();++it)
	{
		//add client
		CXmlElement single_client = users_list.InsertChild(EIB_SERVER_USER_XML);
				
		//add client data
		single_client.InsertChild(EIB_SERVER_USER_NAME_XML).SetValue((*it)._name);
		single_client.InsertChild(EIB_SERVER_USER_PASSWORD_XML).SetValue((*it)._password);
		single_client.InsertChild(EIB_SERVER_USER_IS_CONNECTED_XML).SetValue((*it)._connected);
		single_client.InsertChild(EIB_SERVER_USER_SESSION_ID_XML).SetValue((*it)._session_id);
		single_client.InsertChild(EIB_SERVER_USER_PRIVILIGES_XML).SetValue((*it)._priviliges);
		single_client.InsertChild(EIB_SERVER_USER_IP_ADDRESS_XML).SetValue((*it)._ip_address);
		//src addr mask
		single_client.InsertChild(EIB_SERVER_USER_SOURCE_ADDR_MASK_XML).SetValue((*it)._sa_mask);
		//dst addr mask
		single_client.InsertChild(EIB_SERVER_USER_DST_ADDR_MASK_XML).SetValue((*it)._da_mask);

	}

	_doc.ToString(xml_str);
}

void CEIBServerUsersConf::FromXml(const CDataBuffer& xml_str)
{
	_clients.clear();
	try
	{
		_doc.Parse((const char*)xml_str.GetBuffer());
		if(!_doc.RootElement().FirstChildElement(EIB_SERVER_USERS_LIST_XML).IsValid()){
			//send error xml to console
			return;
		}
		CXmlElement users_list_elem = _doc.RootElement().FirstChildElement(EIB_SERVER_USERS_LIST_XML);
		CXmlElement client_elem = users_list_elem.FirstChildElement(EIB_SERVER_USER_XML);
		do
		{
			CString name = client_elem.FirstChildElement(EIB_SERVER_USER_NAME_XML).GetValue();
			CString password = client_elem.FirstChildElement(EIB_SERVER_USER_PASSWORD_XML).GetValue();
			CString privliges = client_elem.FirstChildElement(EIB_SERVER_USER_PRIVILIGES_XML).GetValue();
			CString src_mask = client_elem.FirstChildElement(EIB_SERVER_USER_SOURCE_ADDR_MASK_XML).GetValue();
			CString dst_mask = client_elem.FirstChildElement(EIB_SERVER_USER_DST_ADDR_MASK_XML).GetValue();

			CClientConf single_client;
			
			single_client._name = name;
			single_client._password = password;
			single_client._priviliges = privliges.ToInt();
			single_client._sa_mask = src_mask.ToUShort();
			single_client._da_mask = dst_mask.ToUShort();

			_clients.insert(_clients.begin(),single_client);

			client_elem = client_elem.NextSibling(EIB_SERVER_USER_XML);

		}while(client_elem.IsValid());
	}
	catch (CEIBException& e)
	{
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Error parsing in XML file: %s", e.what());
	}
}

void CEIBServerUsersConf::SetConnectedClients()
{
	CUsersDB users;

	list<CClientConf>::iterator list_it = _clients.begin();

	for(list_it = _clients.begin();list_it!= _clients.end();++list_it)
	{
		CUser current;

		current.SetName(list_it->_name);
		current.SetPriviliges(list_it->_priviliges);
		current.SetPassword(list_it->_password);
		current.SetAllowedDestAddressMask(list_it->_da_mask);
		current.SetAllowedSourceAddressMask(list_it->_sa_mask);

		users.AddRecord(current.GetName(),current);
	}

	//initializing users db
	CString file_name(CURRENT_CONF_FOLDER);
	file_name += DEFAULT_USERS_DB_FILE;
	users.Init(file_name);
	users.Save();

	CEIBServer::GetInstance().ReloadConfiguration();
}

void CEIBServerUsersConf::GetConnectedClients()
{
	CString client_ip;
	int session_id;
	CClientConf client;
	const map<CString,CUser>& users = CEIBServer::GetInstance().GetUsersDB().GetUsersList();
	map<CString,CUser>::const_iterator it = users.begin();
    list<CClientConf>::iterator list_it = _clients.begin();

	for(it = users.begin();it!= users.end();++it)
	{	
		client._name = it->second.GetName();
		client._password = it->second.GetPassword();

		client._priviliges = 0;

		if(it->second.IsReadPolicyAllowed())
			client._priviliges |= USER_POLICY_READ_ACCESS;
		if(it->second.IsWritePolicyAllowed())
			client._priviliges |= USER_POLICY_WRITE_ACCESS;
		if(it->second.IsConsoleAccessAllowed())
			client._priviliges |= USER_POLICY_CONSOLE_ACCESS;

		client._sa_mask = it->second.GetSrcMask();
		client._da_mask = it->second.GetDstMask();

		if (CEIBServer::GetInstance().GetClientsManager()->IsClientConnected((*it).first,client_ip,session_id)){
			client._connected = true;
			client._ip_address = client_ip;
			client._session_id = session_id;
		}
		else{
			client._connected = false;
			client._ip_address = EMPTY_STRING;
			client._session_id = -1;
		}
		
		//client._is_current_user = (client._name == current_name);
		_clients.insert(_clients.begin(),client);
	}
}

