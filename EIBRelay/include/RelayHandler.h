#ifndef __RELAY_HANDLER_HEADER__
#define __RELAY_HANDLER_HEADER__

#include "RelayServerConfig.h"
#include "JTC.h"
#include "GenericServer.h"
#include "Socket.h"

class CRelayControlHandler;
class CRelayDataInputHandler;
class CRelayDataOutputHandler;

class CRelayHandler : public CGenericServer
{
public:
	CRelayHandler();
	virtual ~CRelayHandler();
	virtual void Start();
	void Init(CRelayServerConfig* server_conf, CLogFile* log_file);
	void Close();
	bool Connect();

	const CString& GetLocalCtrlAddr() const { return _control_handler->_ctrl_addr; }
	int GetLocalCtrlPort() const { return _control_handler->_ctrl_port; }

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
		//CString _remote_data_addr; YGYG : should we still keep that?
		//int 	_remote_data_port; YGYG : should we still keep that?
	}ConnectionState;

public:
	class CRelayControlHandler : public JTCThread
	{
	public:
		CRelayControlHandler();
		virtual ~CRelayControlHandler();
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
		CString _ctrl_addr;
		int _ctrl_port;
	};

	//This handler will be response of receiving data from the EIBServer and writing 
	//out this data to the connected client
	class CRelayDataOutputHandler : public JTCThread
	{
	public:
		CRelayDataOutputHandler();
		virtual ~CRelayDataOutputHandler();
		virtual void run();
		void Close();
		friend class CRelayHandler;
	private:
		CRelayHandler* _relay;
		bool _stop;
	};

	typedef JTCHandleT<CRelayHandler::CRelayControlHandler> CRelayControlHandlerHandle;
	typedef JTCHandleT<CRelayHandler::CRelayDataOutputHandler> CRelayDataOutputHandlerHandle;

	friend class CRelayControlHandler;
	friend class CRelayDataOutputHandler;

private:
	CRelayServerConfig* _server_conf;
	CLogFile* _log_file;
	ConnectionState _state;
	JTCMonitor _state_monitor;
	CRelayControlHandlerHandle _control_handler;
	CRelayDataOutputHandlerHandle _data_output_handler;
};

#endif
