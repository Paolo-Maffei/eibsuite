#ifndef __RELAY_HANDLER_HEADER__
#define __RELAY_HANDLER_HEADER__

#include "RelayServerConfig.h"
#include "JTC.h"
#include "GenericServer.h"
#include "Socket.h"

#define MAX_CONNS 10

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

	const CString& GetLocalCtrlAddr() const { return _input_handler->GetLocalCtrlAddr(); }
	int GetLocalCtrlPort() const { return _input_handler->GetLocalCtrlPort(); }
	
private:
	typedef struct
	{
		bool		  is_connected;
		unsigned char id;
		unsigned char channelid;
		unsigned char recv_sequence;
		unsigned char send_sequence;
		CString _remote_ctrl_addr;
		int 	_remote_ctrl_port;
		CString _remote_data_addr;
		int 	_remote_data_port;
		JTCMonitor state_monitor;
		CTime 	   _timeout;
	}ConnectionState;

private:
	void InitState(ConnectionState* s);
	void OnClientConnectionClose();

public:
	class CRelayInputHandler : public JTCThread
	{
	public:
		CRelayInputHandler();
		virtual ~CRelayInputHandler();
		virtual void run();
		void Close();
		void Init();

		int GetLocalCtrlPort() const { return _local_port; }
		const CString& GetLocalCtrlAddr() const { return _local_addr; }
		void SetParent(CRelayHandler* relay) { _relay = relay; }

		void SendTunnelToClient(const CCemi_L_Data_Frame& frame, ConnectionState* s);

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
		void SetParent(CRelayHandler* relay) { _relay = relay; }

	private:
		CRelayHandler* _relay;
		bool _stop;
	};

	typedef JTCHandleT<CRelayHandler::CRelayInputHandler> CRelayInputHandlerHandle;
	typedef JTCHandleT<CRelayHandler::CRelayOutputHandler> CRelayOutputHandlerHandle;

private:
	void SendTunnelToClient(const CCemi_L_Data_Frame& frame, ConnectionState* s) { _input_handler->SendTunnelToClient(frame, s); }

public:
	void Broadcast(const CCemi_L_Data_Frame& frame);
	ConnectionState* GetState(int channel);
	ConnectionState* AllocateNewState(const CString& source_ip, int sourc_port);
	void FreeConnection(ConnectionState* s);

private:
	CRelayServerConfig* _server_conf;
	CLogFile* _log_file;
	CRelayInputHandlerHandle _input_handler;
	CRelayOutputHandlerHandle _data_output_handler;
	ConnectionState* _states[MAX_CONNS];
};

#endif
