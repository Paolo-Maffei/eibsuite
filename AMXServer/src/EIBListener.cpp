#include "EIBListener.h"
#include "AMXServer.h"

CEIBListener::CEIBListener() : 
_stop(false)
{
}

CEIBListener::~CEIBListener()
{
}

void CEIBListener::Close()
{
	JTCSynchronized sync(*this);
	_stop = true;
}

void CEIBListener::run()
{
	JTCSynchronized sync(*this);
	
	CEibAddress func;
	unsigned char val[MAX_EIB_VALUE_LEN];
	unsigned char val_len = 0;
	int length;

	while (!_stop)
	{
		length = CAMXServer::GetInstance().ReceiveEIBNetwork(func,val,val_len,2000);
		if (length > 0)
		{
			EIBENTRY entry;
			entry._function = func.ToByteArray();
			entry._value = val[0];
			AMXENTRY result;

			
			if(CAMXServer::GetInstance().GetMsgsTable().FindByRight(entry,result))
			{
				CAMXServer::GetInstance().GetAMXListenr().SendAMXMessage(result._device_id,result._press);
			}
		}

		if(!CAMXServer::GetInstance().IsConnected()){
			CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"EIB Server is disconnected. Stopping EIB Listener...");
			break;
		}

		this->wait(10);
	}
}

