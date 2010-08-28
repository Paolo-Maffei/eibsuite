#include "EIBInterface.h"
#include "EIBServer.h"

CEIBInterface::CEIBInterface() : 
_mode(UNDEFINED_MODE),
_connection(NULL)
{
	_stats._last_time_recevied.SetTime(0);
	_stats._last_time_sent.SetTime(0);
	_stats._total_sent = 0;
	_stats._total_received = 0;

	_info.IsValid = false;
	_info.SupportedServices = 0;
}

CEIBInterface::~CEIBInterface()
{
	if(_connection != NULL){
		delete _connection;
		_connection = NULL;
	}
}

IConnection* CEIBInterface::GetConnection()
{
	return _connection;
}

void CEIBInterface::Close()
{
	if(_connection == NULL){
		return;
	}
	_connection->DisConnect();
}

void CEIBInterface::Init()
{
	CServerConfig& conf = CEIBServer::GetInstance().GetConfig();
	
	CString device_mode = conf.GetEibDeviceMode();
	device_mode.ToUpper();
	//read device mode from conf file
	if (device_mode == EIB_DEVICE_MODE_ROUTING_STR){
		_mode = MODE_ROUTING;
	}else if (device_mode == EIB_DEVICE_MODE_TUNNELING_STR){
		_mode = MODE_TUNNELING;
	}else if(device_mode == EIB_DEVICE_MODE_BUSMONITOR_STR){
		_mode = MODE_BUSMONITOR;
	}

	
	//read the if name from conf file and get the ip address of the card
	CString local_address(Socket::LocalAddress(conf.GetEibLocalInterface()));
	bool result = false;
	//initialize
	if(_connection != NULL){
		delete _connection;
		_connection = NULL;
	}

	switch (_mode)
	{
	case MODE_ROUTING:
		_connection = new CRoutingConnection(local_address);
		result = _connection->Connect();
		break;
	case MODE_TUNNELING:
		_connection = new CTunnelingConnection(local_address);
		result = _connection->Connect();
		break;
	case MODE_BUSMONITOR:
		_connection = new CBusMonConnection(local_address);
		result = _connection->Connect();
		break;
	default: 
		throw CEIBException(ConfigFileError,"Unknown EIB_DEVICE_MODE parameter");
		break;
	}
	if(result){
		LOG_INFO("Remote EIB Interface DATA channel [%s:%d]",_connection->GetDeviceDataAddress().GetBuffer(),_connection->GetDeviceDataPort());
		LOG_INFO("Remote EIB Interface CONTROL channel [%s:%d]",_connection->GetDeviceControlAddress().GetBuffer(),_connection->GetDeviceControlPort());
		LOG_INFO("Local Interface Used to connect to EIB Device [%s:%d]",local_address.GetBuffer(),_connection->GetLocalPort());
	}
	else{
		throw CEIBException(GeneralError,"Connection to EIB Interface failed.");
	}
}

bool CEIBInterface::Read(CCemi_L_Data_Frame& frame)
{
	if(_connection == NULL || !_connection->ReceiveDataFrame(frame)){
		return false;
	}
	//update stats
	++_stats._total_received;
	_stats._last_time_recevied.SetNow();
	return true;
}

void CEIBInterface::Write(const KnxElementQueue& elem)
{
	if(_connection == NULL || _mode == MODE_BUSMONITOR){
		return;
	}

	if(!_connection->SendDataFrame(elem)){
		throw CEIBException(EibPacketError,"Error during send of cEMI Frame...");
	}
	//update stats
	++_stats._total_sent;
	_stats._last_time_sent.SetNow();
}
