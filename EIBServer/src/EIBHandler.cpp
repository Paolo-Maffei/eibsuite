#include "EIBHandler.h"
#include "EIBServer.h"

CEIBHandler::CEIBHandler(HANDLER_TYPE type) : 
JTCThread("EIB Handler"),
_type(type),
_stop(false),
_pause(false)
{
}

CEIBHandler::~CEIBHandler(void)
{
}

void CEIBHandler::run()
{
	switch (_type)
	{
	case INPUT_HANDLER: 
		this->setName("EIB Input handler");
		RunEIBReader();
		break;
	case OUTPUT_HANDLER: 
		this->setName("EIB Output handler");
		RunEIBWriter();
		break;
	}
}

void CEIBHandler::RunEIBReader()
{
	CCemi_L_Data_Frame msg;
	CStatsDB& stats_db = CEIBServer::GetInstance().GetStatsDB();
	
	CEIBInterface& eib_ifc = CEIBServer::GetInstance().GetEIBInterface();
	CClientsMgrHandle& c_mgr = CEIBServer::GetInstance().GetClientsManager();
	
	JTCSynchronized sync(_wait_mon);
	

	while (!_stop)
	{
		START_TRY

			if(eib_ifc.Read(msg))
			{
				//insert packet statistics
				unsigned char value_of_pkt[MAX_EIB_VALUE_LEN];
				msg.FillBufferWithFrameData(value_of_pkt,MAX_EIB_VALUE_LEN);
			
				stats_db.AddRecord(msg.GetDestAddress(),value_of_pkt,msg.GetValueLength());
				//forward packet to all connected clients (i.e. WEBServer, SMSServer etc.)
				c_mgr->Brodcast(msg);
			}

			if(_pause)
			{
				LOG_DEBUG("EIB Reader suspended.");
				_wait_mon.wait();
				LOG_DEBUG("EIB Reader resumed.");
			}

		END_TRY_START_CATCH(e)
			LOG_ERROR("Exception at EIB Reader handler: %s",e.what());
		END_TRY_START_CATCH_SOCKET(ex)
			LOG_ERROR("Socket Exception at EIB Reader handler: %s",ex.what());
		END_TRY_START_CATCH_JTC(ex1)
			LOG_ERROR("JTC Exception at EIB Reader handler: %s",ex1.getMessage());
		END_TRY_START_CATCH_ANY
			LOG_ERROR("Unknown Exception at EIB Reader handler");
		END_CATCH
	}
}

//this method is called from client thread!!! (not from eibhandler thread)
void CEIBHandler::Write(const CCemi_L_Data_Frame& data, BlockingMode mode, JTCMonitor* optional_mon)
{
	do
	{
		//put the frame (that about the be sent) in the Handler queue and wake him up
		//note: writing to the queue is synced
		JTCSynchronized _sync(*this);
		KnxElementQueue elem;
		elem._frame = data;
		elem._mode = mode;
		elem._optional_mon = optional_mon;
		_buffer.Write(elem);
		this->notify();
	}while(0);

	if((mode == WAIT_FOR_CONFRM || mode == WAIT_FOR_ACK) && optional_mon != NULL){
		//block the client thread till we got proper response from the KnxNet/IP device
		JTCSynchronized sync(*optional_mon);
		optional_mon->wait();
	}
}

void CEIBHandler::RunEIBWriter()
{
	KnxElementQueue msg2write;
	CEIBInterface& iface = CEIBServer::GetInstance().GetEIBInterface();

	JTCSynchronized sync(_wait_mon);
	//sync the reading from the queue
	JTCSynchronized _sync(*this);

	while (!_stop)
	{
		START_TRY
			//if we have a packet waiting in the queue then forward it to the KNXNet/IP device
			if(_buffer.Read(msg2write))
			{
				iface.Write(msg2write);
			}

			if(_pause)
			{
				LOG_DEBUG("EIB Writer suspended.");
				_wait_mon.wait();
				LOG_DEBUG("EIB Writer resumed.");
			}

			//release the lock untill the buffer will be filled up or 1 sec passed (to give change to the pause feature)
			this->wait(1500);
		END_TRY_START_CATCH_JTC(e)
			LOG_ERROR("JTC Error: %s",e.getMessage());
		END_TRY_START_CATCH(ex)
			LOG_ERROR("Exception at EIB Writer handler: %s",ex.what());
		END_TRY_START_CATCH_ANY
			LOG_ERROR("Unknown Exception at EIB Writer handler");
		END_CATCH
	}
}

void CEIBHandler::Close()
{
	_stop = true;
}

void CEIBHandler::Suspend()
{
	_pause = true;
	JTCSynchronized sync(_wait_mon);
}

void CEIBHandler::Resume()
{
	_pause = false;
	JTCSynchronized sync(_wait_mon);
	_wait_mon.notify();
}
