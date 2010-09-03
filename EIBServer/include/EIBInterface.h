/*! \file EIBInterface.h
    \brief Defines EIBInterface
    
	This is the header file for EIBInterface

*/
#ifndef __EIB_INTERFACE_HEADER__
#define __EIB_INTERFACE_HEADER__

#include <iostream>
#include "Socket.h"
#include "EibNetwork.h"
#include "DataBuffer.h"
#include "ServerConfig.h"
#include "EIBNetIP.h"
#include "CTime.h"
#include "TunnelConnection.h"
#include "RoutingConnection.h"
#include "BusMonConnection.h"
#include "EIBHandler.h"

#define EIB_DEVICE_MODE_TUNNELING_STR "MODE_TUNNELING"
#define EIB_DEVICE_MODE_ROUTING_STR "MODE_ROUTING"
#define EIB_DEVICE_MODE_BUSMONITOR_STR "MODE_BUSMONITOR"

typedef struct EIBInterfaceStats
{
	CTime _last_time_sent;
	CTime _last_time_recevied;
	int _total_sent;
	int _total_received;
}EIBInterfaceStats;

typedef struct EIBInterfaceInfo
{
	bool IsValid;
	CString Name;
	CString MulticastAddr;
	CString MACAddr;
	CString SerialNumber;
	int SupportedServices;
	CEibAddress KNXAddress;

}EIBInterfaceInfo;

typedef JTCHandleT<CEIBHandler> CEIBHandlerHandle;

/*! \class CEIBInterface
	\brief Provides an interface between the EIB Server and the EIB System.

	This object is a singleton, meaning that the EIB Server will create an instance of CEIBInterface and no
	additional instances can be created.\n
	CEIBInterface provides the basic functions needed to interface with the EIB Instabus.\n
	Currently the class supports only Ethernet, but additional support for RS232 is planned.
*/
class CEIBInterface
{
public:
	/*!
		\fn CEIBInterface(void)

		private constructor
	*/
	CEIBInterface();
	/*!
		 destructor
	*/
	virtual ~CEIBInterface();
	/*!
		\fn void Init()
		\brief Initializes the interface
	*/
	void Init();
	/*!
		\fn void Close()
		\brief Closes the interface
	*/
	void Close();

	void Start();
	/*!
		\fn Read(EibMsg& msg)
		\brief Reads data coming from the EIB Instabus
		\param msg Data structure of eib message to fill with the data from interface
	*/
	bool Read(CCemi_L_Data_Frame& msg);//, const CString& local_address);
	/*!
		\fn void Write(const KnxElementQueue& elem)
		\brief Writes a message to the EIB interface
		\param elem Reference to the Element in queue that will be written to the EIB Instabus
	*/	
	void Write(const KnxElementQueue& elem);
	
	/*!
		\fn inline CEIBHandler& GetInputHandler()
		Returns reference to Input Handler
	*/
	inline CEIBHandlerHandle& GetInputHandler() { return _input_handler;}
	/*!
		\fn inline CEIBHandler& GetOutputHandler()
		Returns reference to Output Handler
	*/
	inline CEIBHandlerHandle& GetOutputHandler() { return _output_handler;}

	EIB_DEVICE_MODE GetMode() const { return _mode;}

	void SetDefaultPacketFields(CCemi_L_Data_Frame& msg);
	IConnection* GetConnection();

	const EIBInterfaceStats& GetInterfaceStats() { return _stats; }
	const EIBInterfaceInfo& GetInterfaceInfo() { return _info; }
	void SetInterfaceInfo(EIBInterfaceInfo& info) { _info = info; }

private:
	//device mode
	EIB_DEVICE_MODE _mode;
	//the connection
	IConnection* _connection;
	CEIBHandlerHandle _input_handler;
	CEIBHandlerHandle _output_handler;
	//statistics object
	EIBInterfaceStats _stats;
	EIBInterfaceInfo _info;
};

#endif
