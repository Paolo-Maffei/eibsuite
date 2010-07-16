/*! \file ClientsMgr.h
    \brief Defines CClientsMgr
    
	This is the header file for CListenerThread and CClient.

*/
#ifndef __CLIENTS_MANAGER_HEADER__
#define __CLIENTS_MANAGER_HEADER__

#include <iostream>
#include <map>
#include <stack>
#include <list>
#include "CString.h"
#include "Client.h"
#include "Socket.h"
#include "JTC.h"
#include "ServerConfig.h"
#include "Protocol.h"
#include "DataBuffer.h"
#include "EIBNetIP.h"
#include "CMutex.h"

typedef JTCHandleT<CClient> CClientHandle;

using namespace std;

/*! \class CClientsMgr
	\brief Handles new connections and disconnections of clients

	CClientsMgr is a thread that manages the list of connected clients.
	The thread accepts new connections and handles disconnection of connected clients.
*/
class CClientsMgr : public JTCThread, public JTCMonitor
{
public:
	/*!
		constructor
		\param conf Reference to config file that contains communication parameters
	*/	
	CClientsMgr();
	/*!
		 destructor
	*/
	virtual ~CClientsMgr(void);
	/*!
		\brief Starting point for thread, listens for new connections from clients
		\fn virtual void Run(void* arg) 
	*/
	void run();
	/*!
		\fn void Brodcast(EibMsg& msg)
		\brief Broadcasts a message to all clients that have reading privileges
		\param msg reference to the broacast message	
	*/	
	void Brodcast(CCemiFrame& msg);
	/*!
		\fn void Disconnect(const CClient& terminate_client)
		\brief Disconnects a client	 
		\param terminate_client Reference to the client that is to be dssconnected
	*/
	void Disconnect(int session_id); // close specific client
	/*!
		\fn void Close()
		\brief Disconnects all clients and shuts down
	*/
	void Close(); //shut down

	void Init();
	int GetNumConnectedClients();

	const CString GetListeningAddress() const { return _local_address; }

	bool IsClientConnected(const CString& client_name,CString& client_ip,int& session_id);

private:
	void InitClient(CString& source_address,int source_port,int keep_alive_port);
	bool IsOpenConnectionMessage(char* data, int length,CString& source_address,int& source_port,int& keep_alive_port);
	int GetSessionID();
	void HandleServiceDiscovery(char* buffer, int maxlen);

private:
	UDPSocket _server_sock; //!socket to listen for new clients
	UDPSocket _broadcast_sock; //! socket to serve "Auto discovery" service requests
	map<int,CClientHandle> _clients; //! hash table of connected clients
	bool _stop;
	CString _local_address;
	bool _auto_discovery_enabled;
};

#endif
