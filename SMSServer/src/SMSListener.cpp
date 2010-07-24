#include "SMSListener.h"
#include "SMSServer.h"
#include "gsm_util.h"

using namespace gsmlib;

CSMSListener::CSMSListener() : _stop(false)
{
}

CSMSListener::~CSMSListener()
{
}

void CSMSListener::Close()
{
	_stop = true;
	this->join();
}

void CSMSListener::run()
{
	ASSERT_ERROR(global_meta != NULL,"ME/TA cannot be NULL");
	
	EventHandler eh;
	global_meta->setEventHandler(&eh);

	CMutex& lock = CSMSServer::GetInstance().GetMetaLock();

	while (!_stop)
	{
		//JTCThread::sleep(1000);
		lock.Lock();
#ifdef WIN32
		::timeval timeoutVal;
		timeoutVal.tv_sec = 1;
		timeoutVal.tv_usec = 0;
		global_meta->waitEvent((gsmlib::timeval *)&timeoutVal);
#else
		struct timeval timeoutVal;
		timeoutVal.tv_sec = 1;
		timeoutVal.tv_usec = 0;
		global_meta->waitEvent(&timeoutVal);
#endif
		CheckForNewMessages(eh,global_meta);
		lock.Release();
	}
	
}

void CSMSListener::CheckForNewMessages(EventHandler& event_handler,MeTa* me)
{
	if(event_handler.GetNumNewMsgs() <= 0){
		return;
	}
	
	// get first new message and remove it from the vector
    SMSMessageRef newSMSMessage = event_handler.GetNewMessages().begin()->_newSMSMessage;
    CBMessageRef newCBMessage = event_handler.GetNewMessages().begin()->_newCBMessage;
    GsmEvent::SMSMessageType messageType = event_handler.GetNewMessages().begin()->_messageType;
    int index = event_handler.GetNewMessages().begin()->_index;
    string storeName = event_handler.GetNewMessages().begin()->_storeName;
    event_handler.GetNewMessages().erase(event_handler.GetNewMessages().begin());

	CString text;
	CString origin_number;

    if (! newSMSMessage.isnull())
	{
          text = newSMSMessage->userData();
		  origin_number = newSMSMessage->address()._number;
	}
    else if (! newCBMessage.isnull())
	{
		text = newCBMessage->getData();
	}
    else
    {
          SMSStoreRef store = me->getSMSStore(storeName);
          store->setCaching(false);

          if (messageType == GsmEvent::CellBroadcastSMS)
			  text = (*store.getptr())[index].cbMessage()->getData();
          else
			  text = (*store.getptr())[index].message()->userData();
			  origin_number = (*store.getptr())[index].message()->address()._number;
            
          store->erase(store->begin() + index);
    }

	//look for the sms that received in the db
	list<CUserAlertRecord> res;
	if(CSMSServer::GetInstance().GetDB().FindEibMessages(text,res)){
		list<CUserAlertRecord>::iterator it;
		for(it = res.begin(); it != res.end(); ++it)
		{
			if(origin_number != it->GetPoneNumber()){
				continue;
			}
			
			unsigned char val[1];
			val[0] = it->GetValue();
			unsigned char val_len = 1;

			CSMSServer::GetInstance().GetEIBAgent().SendEIBNetwork(
					CEibAddress(it->GetDestAddress(),true),
					val,
					val_len,
					NON_BLOCKING
					);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void EventHandler::SMSReception(SMSMessageRef newMessage, SMSMessageType messageType)
{
	IncomingMessage m;
	m._messageType = messageType;
	m._newSMSMessage = newMessage;
	_newMessages.push_back(m);
}

void EventHandler::CBReception(CBMessageRef newMessage)
{
	IncomingMessage m;
	m._messageType = GsmEvent::CellBroadcastSMS;
	m._newCBMessage = newMessage;
	_newMessages.push_back(m);
}

void EventHandler::SMSReceptionIndication(string storeName, unsigned int index,SMSMessageType messageType)
{
  IncomingMessage m;
  m._index = index;

  m._storeName = storeName;

  m._messageType = messageType;
  _newMessages.push_back(m);
}
