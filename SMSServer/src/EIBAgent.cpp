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
	_stop = true;
	//wait for the thread to quit
	this->join();
}

bool CEIBAgent::ConnectToEIB()
{
	CLogFile& log = CSMSServer::GetInstance().GetLog();
	CGenericServer::Init(&log);

	CSMSServerConfig& conf = CSMSServer::GetInstance().GetConf();

	bool established = false;
	if(conf.GetAutoDiscoverEibServer())
	{
		log.Log(LOG_LEVEL_INFO,"Searching EIB Server on local network...");
		established = this->OpenConnection(conf.GetNetworkName().GetBuffer(),
											conf.GetInitialKey().GetBuffer(),
											Socket::LocalAddress(conf.GetListenInterface()).GetBuffer(),
											conf.GetName().GetBuffer(),
											conf.GetPassword().GetBuffer());
	}
	else
	{
		established = this->OpenConnection(conf.GetNetworkName().GetBuffer(),
											conf.GetEibIPAddress(),
											conf.GetEibPort(),
											conf.GetInitialKey().GetBuffer(),
											Socket::LocalAddress(conf.GetListenInterface()).GetBuffer(),
											conf.GetName().GetBuffer(),
											conf.GetPassword().GetBuffer());
	}
	return established;
}

void CEIBAgent::run()
{
	CEibAddress func;
	unsigned char val[5];
	unsigned char val_len = 0;
	int length;

	SerialPort* cell_port = CSMSServer::GetInstance().GetSerialPort();
	CSMSServerDB& db = CSMSServer::GetInstance().GetDB();

	while (!_stop)
	{
		length = ReceiveEIBNetwork(func,val,val_len, 1000);

		if (length > 0)
		{
			cout << endl;
			cout << "Received " << length << " Bytes from EIB Server." << endl;

			list<CUserAlertRecord> result;
			if(cell_port != NULL && db.FindSmsMesaages(func.ToByteArray(),val[0],result)){
				list<CUserAlertRecord>::iterator it;
				for(it = result.begin(); it != result.end(); ++it)
				{
					CSMSServer::GetInstance().GetMetaLock().Lock();
					CSMSServer::GetInstance().SendSMS(it->GetPoneNumber(),it->GetTextMessage());
					CSMSServer::GetInstance().GetMetaLock().Release();
				}
			}
		}
	}
}
