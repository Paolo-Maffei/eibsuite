/*! \file GenericServer.h
    \brief Generic Server Class - Source file

	This is The header file for CDiffieHellman class. This class serves as the base class for all clients wish
	to connect to the EIB Server (i.e. SMSServer, PMServer etc.). although it is not a must to use this class as
	a base class for all clients, it be much easier.

*/
#ifndef __GENERICSERVER_HEADER__
#define __GENERICSERVER_HEADER__

#include "EibStdLib.h"
#include "Socket.h"
#include "CString.h"
#include "DataBuffer.h"
#include "EibNetwork.h"
#include "LogFile.h"
#include "DiffieHellman.h"
#include "Protocol.h"
#include "HttpParser.h"
#include "EIBAddress.h"
#include "JTC.h"
#include "CCemi_L_Data_Frame.h"

using namespace EibStack;
using namespace std;

typedef int STATUS_MASK;

enum EIB_STD_EXPORT ServerStatus
{
	STATUS_DISCONNECTED,
	STATUS_CONNECTED,
	STATUS_DURING_CONNECT,
	STATUS_DURING_DISCONNECT
};

class CGenericServer;
class CDiffieHellman;

/*! \class CHeartBeatThread
	\brief HeartBeat Thread class. this class is responsible to implement all the neccessary action taken during
		   the diffe-hellman protocol. this class genrate random numbers acroding to the CPU time stamp.
		   (EAX opcode in the x86 processors). to generate large random number we use integer in 64 bit size. in addition,
		   Miller-Rabin algorithm is used to determine whether a number is prime or not. the class can handle all
		   the Connection establishment process, and will execute seperate thread to handle the HeartBeat messages.
*/
class EIB_STD_EXPORT CHeartBeatThread : public JTCThread, public JTCMonitor
{
public:
	CHeartBeatThread();
	virtual ~CHeartBeatThread();

	/*!
	\brief Method to initialized the class members.
	\fn void Init(int server_port,const CString& server_address, int heartbeat_interval)
	\param server_port the port in wich the EIB Server listen for HeartBeat messages
	\param server_address address of the eib server.
	\param heartbeat_interval the time interval to send Heartbeat messages.
	*/
	void Init(int server_port,const CString& server_address,CGenericServer* parent);
	/*!
	\brief Method inherited from CThread. this method performs the thread work and will send HeartBeat Messages
		   every time interval. after heartbeat packet send the thread will wait for an ACK to the sent message.
	\fn virtual void Run(void* arg)
	\param arg pointer to the Generic server instance.
	*/
	virtual void run();
	/*!
	\brief Get Method
	\fn int GetHeartBeatPort()
	\return the port number in wich the thread listens for Heartbeat ACK messages
	*/
	int GetHeartBeatPort() { return _sock.GetLocalPort();}

	void Close();

	friend class CGenericServer;
private:
	int _server_port;
	unsigned char _network_id;
	int _heartbeat_interval;
	CString _server_address;
	UDPSocket _sock;
	CGenericServer* _parent;
	bool _stop;
};

typedef EIB_STD_EXPORT JTCHandleT<CHeartBeatThread> CHeartBeatThreadHandle;

/*!
	\class CGenericServer
	\brief Generic Server class. this class is responsible to implement all the neccessary action taken during
		   the diffe-hellman protocol. this class genrate random numbers acroding to the CPU time stamp.
		   (EAX opcode in the x86 processors). to generate large random number we use integer in 64 bit size. in addition,
		   Miller-Rabin algorithm is used to determine whether a number is prime or not. the class can handle all
		   the Connection establishment process, and will execute seperate thread to handle the HeartBeat messages.
*/
class EIB_STD_EXPORT CGenericServer
{
public:
	CGenericServer(char network_id);
	virtual ~CGenericServer();

public:
	/*!
	\brief Method used to initialize connection between the current client and the eib server. firstly, the method
		   will run the Diffie hellman protocol, and after the keys exchange was successful, the method will try to
		   authenciate the using calss according the supplied credentials. if the client autheticated, the method
		   will execute the keep alive thread and return true. in any other case the method will return false
	\fn bool OpenConnection(const CString& network_name, const CString& eib_server_adress,int eib_server_port,
		const CString& initial_key,const CString& username, const CString& password, int heartbeat_interval)
	\param network_name the Network using the EIB Server name (i.e. SMS)
	\param eib_server_adress EIB Server address
	\param eib_server_port EIB Server port
	\param initial_key the initial encryption key (used untill diffie hellman protocol ends)
	\param username User Name
	\param password Password
	\param heartbeat_interval the time interval to send Heartbeat messages.
	\return true if connection to the eib server established. false else.
	*/
	bool OpenConnection(const CString& network_name, const CString& eib_server_adress,int eib_server_port,
		const CString& initial_key, const CString& local_ip,const CString& username, const CString& password);

	bool OpenConnection(const char* network_name, const char* eib_server_adress,int eib_server_port,
		const char* initial_key,const char* local_ip,const char* username, const char* password);

	bool OpenConnection(const char* network_name, const char* initial_key,
						const char* local_ip, const char* user_name, const char* password);

	/*!
	\brief Method used to send data to the EIB Server. the data must be in form of EibMsg, and the method will
		   add the neccesarry header and encrypt the data before sending it over the network.
	\fn int SendEIBNetwork(const char *buffer, int length)
	\param buffer pointer to the data need to be sent
	\param length the length of the data that need to be sent
	\param mode the sending mode for this request (non blocking, Wait for ack, Wait for confirmation)
	\return int - the number of bytes sent to the EIB Server
	*/
	int SendEIBNetwork(const CEibAddress& address, unsigned char* value, int value_len, BlockingMode mode);
	int SendEIBNetwork(const CCemi_L_Data_Frame& frame, BlockingMode mode);
	/*!
	\brief Method used to send data to the EIB Server. the data must be in form of EibMsg, and the method will
		   add the neccesarry header and encrypt the data before sending it over the network.
	\fn int ReceiveEIBNetwork(char *buffer, int max_length, int timeout = -1);
	\param buffer pointer to memory that should be filled up with the received data
	\param max_length the max length of the data can be stored in the buffer
	\param timeout max time to wait for receive data. when timeout expires, even if no data is present the function will return 0.
	\return int - the number of bytes Received from the EIB Server
	*/
	int ReceiveEIBNetwork(CEibAddress& function, unsigned char* value, unsigned char& value_len, int timeout = INFINITE);
	int ReceiveEIBNetwork(CCemi_L_Data_Frame& frame, int timeout);
	/*!
	\brief Get Method
	\fn unsigned char GetNetworkID()
	\return the Netowrk ID (e.g. SMSServer, PMServer etc.)
	*/
	unsigned char GetNetworkID() { return _network_id;}
	/*!
	\brief Get Method
	\fn int GetSessionID()
	\return the client session identifier
	*/
	int GetSessionID() { return _session_id;}
	/*!
	\brief Get Method
	\fn const CString& GetSharedKey()
	\return The shared key generated after the diffie hellman protocol
	*/
	const CString& GetSharedKey();

	/*!
	\brief Get Method
	\fn bool IsConnected()
	\return whether the client is connected to the eib server
	*/
	bool IsConnected();
	/*!
	\brief Set Method
	\fn void SetConnected(bool val)
	\param val the connection state
	*/
	void SetStatus(ServerStatus val) { _status = val;}
	
	void Close();

	void Init(CLogFile* log);

	CLogFile* GetLog() { return _log; }

	const CString& GetUserName() const { return _user_name; }

	void SetConnectionParams(const CString& network_name,
							 const CString& eib_server_adress,
							 int eib_server_port,
							 const CString& initial_key,
							 const CString& local_ip,
							 const CString& user_name,
							 const CString& password);

private:
	bool FirstPhaseConnection(const CString& key,const char* local_ip, char* buff, int buf_len,int& reply_length);
	bool Authenticate(const CString& user_name,const CString& password,const CString* key);

protected:
	UDPSocket _data_sock;
	CString _eib_address;
	int _eib_port;
	char _network_id;
	CDiffieHellman _encryptor;
	
private:
	//bool _connected;
	ServerStatus _status;
	int _session_id;
	CHeartBeatThreadHandle _thread;
	CString _network_name;
	CString _user_name;
	CLogFile* _log;
	JTCInitialize _jtc_init;
	EIB_DEVICE_MODE _ifc_mode;
};


#endif
