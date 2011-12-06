#include "EIBAgent.h"
#include "SMSServerDB.h"
#include "SMSServer.h"

CEIBAgent::CEIBAgent() :
CGenericServer(EIB_TYPE_SMS_SERVER),
_stop(false)
{
}

CEIBAgent::~CEIBAgent()
{
}

void CEIBAgent::Close()
{
	CGenericServer::Close();
	_stop = true;
}

bool CEIBAgent::ConnectToEIB()
{
	CLogFile& log = CSMSServer::GetInstance().GetLog();
	CGenericServer::Init(&log);

	CSMSServerConfig& conf = CSMSServer::GetInstance().GetConf();

	ConnectionResult res;
	CString localIP = Socket::LocalAddress(conf.GetListenInterface());
	CString serverIP = conf.GetEibIPAddress();
	int serverPort = conf.GetEibPort();

	//bool established = false;
	if(conf.GetAutoDiscoverEibServer())
	{
		LOG_INFO("Searching EIB Server on local network...");
		DiscoverEIBServer(localIP,
							conf.GetInitialKey().GetBuffer(),
							serverIP,
							serverPort);
	}
	
	res = OpenConnection(conf.GetNetworkName().GetBuffer(),
							serverIP.GetBuffer(),
							serverPort,
							conf.GetInitialKey().GetBuffer(),
							Socket::LocalAddress(conf.GetListenInterface()).GetBuffer(),
							conf.GetName().GetBuffer(),
							conf.GetPassword().GetBuffer());
	
	return (res == STATUS_CONN_OK);
}

void CEIBAgent::run()
{
	CEibAddress func;
	unsigned char val[5];
	unsigned char val_len = 0;
	int length;

	CSMSServer& singleton = CSMSServer::GetInstance();
	CSMSServerDB& db = singleton.GetDB();

	while (!_stop)
	{
		length = ReceiveEIBNetwork(func,val,val_len, 1000);

		if (length > 0)
		{
			LOG_DEBUG("[Received] CEMI frame from EIB Server. EIBAddress: %s (%d bytes).", func.ToString().GetBuffer(), length);

			list<CUserAlertRecord> result;
			if(db.FindSmsMesaages(func.ToByteArray(),val[0],result)){
				list<CUserAlertRecord>::iterator it;
				for(it = result.begin(); it != result.end(); ++it)
				{
					//JTCSynchronized(*(singleton.GetSMSListener()));
					LOG_DEBUG("Trying to send SMS: %s", it->GetPoneNumber().GetBuffer());
					CSMSServer::GetInstance().SendSMS(it->GetPoneNumber(),it->GetTextMessage());
				}
			}
		}
	}
}
