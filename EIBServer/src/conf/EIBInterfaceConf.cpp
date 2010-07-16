#include "conf/EIBInterfaceConf.h"
#include "EIBServer.h"

CEIBInterfaceConf::CEIBInterfaceConf()
{
}

CEIBInterfaceConf::~CEIBInterfaceConf()
{
}

void CEIBInterfaceConf::ToXml(CDataBuffer& xml_str)
{
	CEIBInterface& eib_interface = CEIBServer::GetInstance().GetEIBInterface();
	
	CXmlElement root = _doc.RootElement();
	//device address
	root.InsertChild(EIB_INTERFACE_ADDRESS_XML).SetValue(eib_interface.GetConnection()->GetDeviceControlAddress());
	//device port
	root.InsertChild(EIB_INTERFACE_PORT_XML).SetValue(eib_interface.GetConnection()->GetDeviceControlPort());
	//working mode
	CString mode_str;
	if(eib_interface.GetMode() == MODE_ROUTING)
	{
		mode_str = "MODE_ROUTING";
	}
	else if(eib_interface.GetMode() == MODE_TUNNELING)
	{
		mode_str = "MODE_TUNNELING";
	}
	else
	{
		mode_str = "MODE_UNKNOWN";
	}
	root.InsertChild(EIB_INTERFACE_DEVICE_MODE_XML).SetValue(mode_str);
	//is autodetect feature is on?
	root.InsertChild(EIB_INTERFACE_AUTO_DETECT_XML).SetValue(CEIBServer::GetInstance().GetConfig().GetAutoDetectEibDeviceAddress());
	//interface stats
	const EIBInterfaceStats& stats = eib_interface.GetInterfaceStats();
	root.InsertChild(EIB_INTERFACE_LAST_TIME_PACKET_SENT_XML).SetValue(stats._last_time_sent.GetTime() ? stats._last_time_sent.Format() : "Never");
	root.InsertChild(EIB_INTERFACE_LAST_TIME_PACKET_RECEIVED_XML).SetValue(stats._last_time_recevied.GetTime() ? stats._last_time_recevied.Format() : "Never");
	root.InsertChild(EIB_INTERFACE_TOTAL_PACKETS_SENT_XML).SetValue(stats._total_sent);
	root.InsertChild(EIB_INTERFACE_TOTAL_PACKETS_RECEIVED_XML).SetValue(stats._total_received);
	root.InsertChild(EIB_INTERFACE_RUNNING_STATUS_XML).SetValue(eib_interface.GetConnection()->IsConnected());
	
	_doc.ToString(xml_str);

}
void CEIBInterfaceConf::FromXml(const CDataBuffer& xml_str)
{
	
}

bool CEIBInterfaceConf::StopInterface(CDataBuffer& xml_str)
{
	CEIBServer::GetInstance().GetInputHandler()->Suspend();
	CEIBServer::GetInstance().GetOutputHandler()->Suspend();
	
	START_TRY
		CEIBServer::GetInstance().GetEIBInterface().Close();
	END_TRY_START_CATCH(e)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",ex.what());
	END_CATCH

	CEIBServer::GetInstance().GetInputHandler()->Resume();
	CEIBServer::GetInstance().GetOutputHandler()->Resume();
	ToXml(xml_str);
	return true;
}

bool CEIBInterfaceConf::StartInterface(CDataBuffer& xml_str)
{
	
	CEIBServer::GetInstance().GetInputHandler()->Suspend();
	CEIBServer::GetInstance().GetOutputHandler()->Suspend();
	
	START_TRY	
		CEIBServer::GetInstance().GetEIBInterface().Init();
	END_TRY_START_CATCH(e)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",ex.what());
	END_CATCH

	CEIBServer::GetInstance().GetInputHandler()->Resume();
	CEIBServer::GetInstance().GetOutputHandler()->Resume();
	ToXml(xml_str);
	return true;
}

