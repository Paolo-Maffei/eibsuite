/*! \file Client.h
    \brief Defines CListenerThread and CClient
    
	This is the header file for CListenerThread and CClient.

*/

#ifndef __CLIENT_HEADER__
#define __CLIENT_HEADER__

#include <iostream>
#include <signal.h>
#include "JTC.h"
#include "Socket.h"
#include "DiffieHellman.h"
#include "Protocol.h"
#include "StringTokenizer.h"
#include "HttpParser.h" 
#include "EibNetwork.h"
#include "CString.h"
#include "Buffer.h"
#include "UsersDB.h"
#include "EIBNetIP.h"
#include "CemiFrame.h"

using namespace std;

#define STATUS_MASK_IS_SET(value,bit) ( (((bit >> 1) & value) == 0) ? false : true )
#define STATUS_MASK_SET(value,bit) (value |= (1 << bit))
#define STATUS_MASK_CLEAR(value,bit) (value &= ~(1 << bit))
#define UNDEFINED_PORT -1

typedef CBuffer<CCemi_L_Data_Frame, 250> CClientBuffer;

#ifdef WIN32
typedef __int64 int64;
#else
typedef int64_t int64;
#endif

using namespace EibStack;

class CClient;

/*! \struct ClientPolicy
    \brief Represents the client's read or write permission
*/
typedef struct ClientPolicy
{
	bool _read;	//!<	Indicates if the policy includes reading privileges
	bool _write;//!<	Indicates if the policy includes writing privileges
}ClientPolicy;

/*! \class CListenerThread
	\brief Manages KeepAlive messages to and from the EIB Server

	CListenerThread maintains a UDP Socket used for the KeepAlive Channel with the EIB Server.
	CListenerThread sends a KeeplAlive message at a configurable interval. CListenerThread awaits an acknowledgement 
	from the EIB Server or a time-out. Timing-out will cause CClient to terminate the connection. 
*/
class CListenerThread : public JTCThread
{
public:
	/*!
		constructor
	*/	
	CListenerThread();
	/*!
		destructor
	*/	
	virtual ~CListenerThread();

	/*!
		\brief Starting point and logic of thread
		\fn virtual void Run(void* arg) 
		\return void

		This function performs the functionality of the CListenerThread.
		The thread waits for an incoming message on the UDP socket _sock. When a message is received, several 
		tests are made to ensure that the message is a KeepAlive message from the EIB Server as expected.
		If the message passes these tests, an ACK message is sent as a reply to the EIB Server.
		If no KeepAlive message is received and time-out expires, the CClient is disconnected by setting it's 
		_logged_in member to false.
	*/
	virtual void run();
	/*!
		\fn int GetListenPort()
		\brief Get listening port
		\return int listening port
	*/
	int GetListenPort() { return _sock.GetLocalPort();}
	
	void Close();

	void SetParent(CClient* parent) { _parent = parent; }

private:
	/*! \var UDPSocket _sock
		\brief The UDP socket that CListenerThread uses for the KeepAlive Channel with the EIB Server
	*/
	UDPSocket _sock;
	CClient* _parent;
	bool _run;
};

#define CLIENT_BUFFER_SIZE 250

typedef JTCHandleT<CListenerThread> CListenerThreadHandle;

/*! \class CClient
	\brief A Client that connect to the EIB Server

	CClient can be any client that connects to the EIB Server. CClients receive messages from the EIB system
	via the EIB Server and can also send messages to the EIB system via the EIB Server.
	Connection with the EIB Server follows the following protocol:\n
	1. Sends public HELLO message encrypted using a pre-defined constant key.\n
	2. Obtains a common key by executing a Diffie-Hellman Key exchange\n
	3. Authentication of client name and password\n
	4. Start a CListenerThread that will maintain KeepAlive messages with the EIB Server\n
*/
class CClient : public JTCThread
{
public:
	/*!
		 constructor
		 \param session_id 
	*/	
	CClient(int session_id);
	/*!
		 destructor
	*/	
	virtual ~CClient(void);
	/*!
		\brief Starting point for thread
		\fn virtual void run() 
	*/
	virtual void run();
	/*!
		\fn void Init(int source_port,int keep_alive_port,CString& source_ip)
		\brief Initializes class members		 
		\param source_port The port that the Client will use for the Data Channel
		\param keep_alive_port The port that the Client will use for the KeepAlive Channel
		\param source_ip The Client's IP address
	*/	
	void Init(int source_port,int keep_alive_port,CString& source_ip);
	/*!
		\fn int GetSessionID() { return _session_id;} const
		\brief Gets _session_id
		\return _session_id
	*/	
	int GetSessionID() { return _session_id;} const
	/*!
		\fn void SetSessionID(int session_id)
		\brief Sets _session_id
		\param session_id Sets _session_id to this value
	*/		
	void SetSessionID(int session_id) { _session_id = session_id;}
	/*!
		\fn void SetLoggedIn(bool val)
		\brief Sets _logged_in
		\param val Sets _logged_in to this value
	*/	
	void SetLoggedIn(bool val) { _logged_in = val;}
	/*!
		\fn bool IsLoggedIn()
		\brief returns _logged_in value
		\return _logged_in
	*/	
	bool IsLoggedIn() { return _logged_in;} 
	
	//client connection parameters
	/*!
		\fn int GetClientIP()
		\brief Gets _client_ip value
		\return _client_ip
	*/
	CString& GetClientIP() { return _client_ip;}
	/*!
		\fn int GetClientPort()
		\brief Gets _client_port value
		\return _client_port
	*/
	int GetClientPort() { return _client_port;}
	/*!
		\fn int GetClientKeepAlivePort()
		\brief Gets _client_ka_port value
		\return _client_ka_port
	*/
	int GetClientKeepAlivePort() { return _client_ka_port;}
	/*!
		\fn void SetClientIP(CString& ip_address)
		\brief Sets _client_ip
		\param ip_address Sets _client_ip to this value
	*/
	void SetClientIP(CString& ip_address) { _client_ip = ip_address;}
	/*!
		\fn void SetClientPort(int port)
		\brief Sets _client_port
		\param port Sets _client_port to this value
	*/
	void SetClientPort(int port) { _client_port = port;}
	/*!
		\fn void SetClientKeepAlivePort(int port)
		\brief Sets _client_ka_port
		\param port Sets _client_ka_port to this value
	*/
	void SetClientKeepAlivePort(int port) { _client_ka_port = port;}
	/*!
		\fn const CString& GetSharedKey()
		\brief Gets value of shared key from _encryptor
		\return CString _shared_key_str
	*/
	const CString& GetSharedKey() { return _encryptor.GetSharedKey();}
	/*!
		\fn unsigned char GetClientType()
		\brief Gets _client_type value
		\return _client_type
	*/
	unsigned char GetClientType() { return _client_type;}
	/*!
		\fn CBuffer& GetBuffer()
		\brief Gets reference to the internal client buffer
		\return _cli_bufferent_port
	*/
	bool InsertToBuffer(CCemi_L_Data_Frame& msg);
	/*!
		\fn const ClientPolicy& GetPolicy()
		\brief Gets reference to the policy instance of the client
		\return _policy
	*/
	const ClientPolicy& GetPolicy() { return _policy;}
	/*!
		\fn void UnregisterClient()
		\brief Remove this client from the list of connected clients (in Clients manager)
		\return void
	*/
	void UnregisterClient();
	/*!
		\fn bool operator==(const CClient& other) const
		\brief compares two CClient objects
		\return bool true iff both objects are identical
	*/
	bool operator==(const CClient& other) const;
	/*!
		\fn bool CanRead() 
		\brief Returns true if client has read privileges
		\return bool
	*/
	bool CanRead()  { return _policy._read;} 
	/*!
		\fn bool CanWrite() 
		\brief Returns true if client has write privileges
		\return bool
	*/
	bool CanWrite() { return _policy._write;}

	const CString& GetName() const {return _client_name;}
	void Close();

private:
	bool ExchangeKeys();
	bool Authenticate(CUser& user);
	void CreatePublicData(CHttpReply& reply);
	void HandleIncomingPktsFromBus(const CUser& user, const CString* key, CCemi_L_Data_Frame& msg);
	void HandleIncomingPktsFromClient(char* buffer, int max_len, const CUser& user, const CString* key, CString& s_address, CCemi_L_Data_Frame& msg);

private:
	bool _logged_in;
	unsigned char _client_type;
	int _session_id;
	UDPSocket _sock;
	CClientBuffer _buffer;
	CListenerThreadHandle _keep_alive_thread;
	CString _client_ip;
	CString _client_name;
	int _client_port;
	int _client_ka_port;
	CDiffieHellman _encryptor;
	ClientPolicy _policy;
	JTCMonitor _pkt_mon;
};

#endif
