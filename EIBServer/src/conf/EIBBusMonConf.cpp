#include "conf/EIBBusMonConf.h"
#include "EIBServer.h"

using namespace std;

CEIBBusMonAddrListConf::CEIBBusMonAddrListConf()
{
}

CEIBBusMonAddrListConf::~CEIBBusMonAddrListConf()
{
}

void CEIBBusMonAddrListConf::ToXml(CDataBuffer &xml_str)
{
	CXmlElement address_list = _doc.RootElement().InsertChild(EIB_BUS_MON_ADDRESSES_LIST_XML);
	
	CStatsDB& db = CEIBServer::GetInstance().GetStatsDB();
	map<CEibAddress,CEIBObjectRecord>::const_iterator it;

	for(it = db.GetData().begin(); it != db.GetData().end() ; ++it)
	{
		CXmlElement address = address_list.InsertChild(EIB_BUS_MON_ADDRESS_XML);
		address.InsertChild(EIB_BUS_MON_ADDRESS_STR_XML).SetValue(it->first.ToString());
		address.InsertChild(EIB_BUS_MON_IS_ADDRESS_LOGICAL_XML).SetValue(it->first.IsGroupAddress());
		
		//add information about the particular addres
		const deque<CEIBRecord>& curr_hist = it->second.GetHistory();
		//the last time that address was seen
		address.InsertChild(EIB_BUS_MON_ADDR_LAST_RECVED_TIME_XML).SetValue(curr_hist.front().GetTime().Format());
		// the last value that was address was seen with
		address.InsertChild(EIB_BUS_MON_LAST_ADDR_VALUE_XML).SetValue(curr_hist.front().PrintValue());
		// how many times we'v seen this address during the current EIBServer up time
		address.InsertChild(EIB_BUS_MON_ADDRESSES_COUNT_XML).SetValue(curr_hist.size());
		
		/*
		deque<CEIBRecord>::const_iterator rec_it;
		const deque<CEIBRecord>& curr_hist = it->second.GetHistory();
		for(rec_it = curr_hist.begin(); rec_it != curr_hist.end(); ++rec_it)
		{
			CXmlElement single_val = records_list.InsertChild(EIB_BUS_MON_ADDR_SINGLE_HISTORY_RECORD_XML);
			single_val.InsertChild(EIB_BUS_MON_ADDR_RECVED_TIME_XML).SetValue(rec_it->GetTime().Format());
			single_val.InsertChild(EIB_BUS_MON_ADDR_VALUE_XML).SetValue(rec_it->PrintValue());
		}
		*/
	}

	_doc.ToString(xml_str);
}

void CEIBBusMonAddrListConf::FromXml(const CDataBuffer &xml_str)
{
}

bool CEIBBusMonAddrListConf::SendCmdToAddr(const CHttpRequest& request)
{
	CHttpParameter dst_addr;
	if(!request.GetParameter(EIB_BUS_MON_SEND_CMD_ADDR_PARAM,dst_addr)){
		return false;
	}
	CHttpParameter send_mode;
	if(!request.GetParameter(EIB_BUS_MON_SEND_MODE_CMD_PARAM,send_mode)){
		return false;
	}
	CHttpParameter cmd_val;
	if(!request.GetParameter(EIB_BUS_MON_SEND_CMD_VAL_PARAM,cmd_val)){
		return false;
	}

	BlockingMode mode = (BlockingMode)send_mode.GetValue().ToInt();

	CEibAddress addr(dst_addr.GetValue());
	//CEIBServer::GetInstance().G

	//build the packet
	CCemi_L_Data_Frame msg;
	msg.SetMessageControl(L_DATA_REQ);
	msg.SetAddilLength(0); //must be 0
	msg.SetCtrl1(0); // init with 0
	msg.SetCtrl2(6); //set the TTL to 6 (Default)
	msg.SetPriority(PRIORITY_NORMAL);
	msg.SetFrameFormatStandard();
	msg.SetSrcAddress(CEibAddress());
	msg.SetDestAddress(addr);
	
	unsigned char data[1] = { 0x81 };
	msg.SetValue(data,1);

	//write the message through eib handler
	CEIBServer::GetInstance().GetOutputHandler()->Write(msg, mode, &_mon);

	return true;
}
