#ifndef __RELAY_HANDLER_HEADER__
#define __RELAY_HANDLER_HEADER__

#include "RelayServerConfig.h"
#include "JTC.h"
#include "GenericServer.h"
#include "Socket.h"

class CRelayInputHandler;
class CRelayDataInputHandler;
class CRelayOutputHandler;

// This class is responsible to maintain a single KNX/IP connection with remote client
class CRelayHandler : public CGenericServer
{
public:
	CRelayHandler();
	virtual ~CRelayHandler();
	virtual void Start();
	void Init(CRelayServerConfig* server_conf, CLogFile* log_file);
	void Close();
	bool Connect();

	const CString& GetLocalCtrlAddr() const { return _input_handler->_local_addr; }
	int GetLocalCtrlPort() const { return _input_handler->_local_port; }
	
private:
	void InitState();
	void OnClientConnectionClose();

private:
	typedef struct
	{
		bool		  _is_connected;
		unsigned char _channelid;
		unsigned char _recv_sequence;
		unsigned char _send_sequence;

		CString _remote_ctrl_addr;
		int 	_remote_ctrl_port;
		CString _remote_data_addr; //??? YGYG : should we still keep that?
		int 	_remote_data_port; //??? YGYG : should we still keep that?

		JTCMonitor _state_monitor;
		CTime 	   _timeout;
	}ConnectionState;

public:
	class CRelayInputHandler : public JTCThread
	{
	public:
		CRelayInputHandler();
		virtual ~CRelayInputHandler();
		virtual void run();
		void Close();
		void Init();



		friend class CRelayHandler;

	private:
		void HandleDisconnectRequest(unsigned char* buffer, int max_len);
		void HandleSearchRequest(unsigned char* buffer, int max_len);
		void HandleConnectRequest(unsigned char* buffer, int max_len);
		void HandleConnectionStateRequest(unsigned char* buffer, int max_len);
		void HandleTunnelRequest(unsigned char* buffer, int max_len);
		void HandleDisconnectResponse(unsigned char* buffer, int max_len);
		void HandleTunnelAck(unsigned char* buffer, int max_len);
		void HandleDescriptionRequest(unsigned char* buffer, int max_len);

	private:
		CRelayHandler* _relay;
		bool _stop;
		UDPSocket _sock;
		CString _local_addr; //used for Control + Data channels
		int _local_port; //used for Control + Data channels
	};

	//This handler will be response of receiving data from the EIBServer and writing 
	//out this data to the connected client
	class CRelayOutputHandler : public JTCThread
	{
	public:
		CRelayOutputHandler();
		virtual ~CRelayOutputHandler();
		virtual void run();
		void Close();
		friend class CRelayHandler;
	private:
		CRelayHandler* _relay;
		bool _stop;
	};

	typedef JTCHandleT<CRelayHandler::CRelayInputHandler> CRelayInputHandlerHandle;
	typedef JTCHandleT<CRelayHandler::CRelayOutputHandler> CRelayOutputHandlerHandle;

	//friend class CRelayInputHandler;
	//friend class CRelayOutputHandler;

public:
	ConnectionState* GetState() { return &_state; }

private:
	CRelayServerConfig* _server_conf;
	CLogFile* _log_file;
	ConnectionState _state;
	CRelayInputHandlerHandle _input_handler;
	CRelayOutputHandlerHandle _data_output_handler;
};

#endif
