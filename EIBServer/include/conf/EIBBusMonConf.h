#ifndef __EIB_BUS_MON_CONF_HEADER__
#define __EIB_BUS_MON_CONF_HEADER__

#include "IConfBase.h"
#include "CTime.h"
#include "StatsDB.h"
#include "HttpRequest.h"
#include "JTC.h"

#define EIB_BUS_MON_ADDRESSES_LIST_XML "EIB_BUS_MON_ADDRESSES_LIST"
#define EIB_BUS_MON_ADDRESS_XML "EIB_BUS_MON_ADDRESS"
#define EIB_BUS_MON_ADDRESS_STR_XML "EIB_BUS_MON_ADDRESS_STR"
#define EIB_BUS_MON_IS_ADDRESS_LOGICAL_XML "EIB_BUS_MON_IS_ADDRESS_LOGICAL"
#define EIB_BUS_MON_ADDR_RECORD_LIST_XML "EIB_BUS_MON_ADDR_RECORD_LIST"
#define EIB_BUS_MON_ADDR_SINGLE_HISTORY_RECORD_XML "EIB_BUS_MON_ADDR_SINGLE_HISTORY_RECORD"
#define EIB_BUS_MON_ADDR_LAST_RECVED_TIME_XML "EIB_BUS_MON_ADDR_LAST_RECVED_TIME"
#define EIB_BUS_MON_LAST_ADDR_VALUE_XML "EIB_BUS_MON_LAST_ADDR_VALUE"
#define EIB_BUS_MON_ADDRESSES_COUNT_XML "EIB_BUS_MON_ADDRESSES_COUNT"
//Send command params
#define EIB_BUS_MON_SEND_CMD_ADDR_PARAM "SND_DEST_ADDR"
#define EIB_BUS_MON_SEND_CMD_VAL_PARAM "SND_VAL"
#define EIB_BUS_MON_SEND_MODE_CMD_PARAM "SND_MODE"

class CEIBBusMonAddrListConf : public IConfBase
{
public:
	CEIBBusMonAddrListConf();
	virtual ~CEIBBusMonAddrListConf();

	virtual void ToXml(CDataBuffer& xml_str);
	virtual void FromXml(const CDataBuffer& xml_str);

	bool SendCmdToAddr(const CHttpRequest& request);
private:
	JTCMonitor _mon;
};

#endif
