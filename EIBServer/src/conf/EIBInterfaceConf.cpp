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
	
	if(eib_interface.GetInterfaceInfo().IsValid)
	{
		CXmlElement infoElem = root.InsertChild(EIB_INTERFACE_DEV_DESCRIPTION_XML);
		const EIBInterfaceInfo& info = eib_interface.GetInterfaceInfo();
		infoElem.InsertChild(EIB_INTERFACE_DEV_NAME_XML).SetValue(info.Name);
		infoElem.InsertChild(EIB_INTERFACE_DEV_MAC_ADDRESS_XML).SetValue(info.MACAddr);
		infoElem.InsertChild(EIB_INTERFACE_DEV_MULTICAST_ADDRESS_XML).SetValue(info.MulticastAddr);
		infoElem.InsertChild(EIB_INTERFACE_DEV_SERIAL_NUMBER_XML).SetValue(info.SerialNumber);
		infoElem.InsertChild(EIB_INTERFACE_DEV_SUPPORTED_SERVICES_XML).SetValue(info.SupportedServices);
		infoElem.InsertChild(EIB_INTERFACE_DEV_PHY_ADDRESS_XML).SetValue(info.KNXAddress.ToString());
	}

	_doc.ToString(xml_str);

}
void CEIBInterfaceConf::FromXml(const CDataBuffer& xml_str)
{
	throw CEIBException(NotImplementedError, "This method is not implemented.");
}

bool CEIBInterfaceConf::StopInterface(CDataBuffer& xml_str)
{
	CEIBInterface& iface = CEIBServer::GetInstance().GetEIBInterface();

	iface.GetInputHandler()->Suspend();
	iface.GetOutputHandler()->Suspend();
	
	START_TRY
		CEIBServer::GetInstance().GetEIBInterface().Close();
	END_TRY_START_CATCH(e)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",ex.what());
	END_CATCH

	iface.GetInputHandler()->Resume();
	iface.GetOutputHandler()->Resume();
	ToXml(xml_str);
	return true;
}

bool CEIBInterfaceConf::StartInterface(CDataBuffer& xml_str)
{
	CEIBInterface& iface = CEIBServer::GetInstance().GetEIBInterface();
	iface.GetInputHandler()->Suspend();
	iface.GetOutputHandler()->Suspend();
	
	START_TRY	
		CEIBServer::GetInstance().GetEIBInterface().Init();
	END_TRY_START_CATCH(e)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		CEIBServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"Console Manager Error [Stop EIB Interface] : %s",ex.what());
	END_CATCH

	iface.GetInputHandler()->Resume();
	iface.GetOutputHandler()->Resume();
	ToXml(xml_str);
	return true;
}

