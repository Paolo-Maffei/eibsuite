#ifndef __SMS_LISTENER_HEADER__
#define __SMS_LISTENER_HEADER__

#include "gsm_config.h"
#include "gsm_nls.h"
#include "gsm_error.h"
#include "gsm_me_ta.h"
#include "gsm_util.h"
#include "CException.h"
#include "Globals.h"
#include "JTC.h"

using namespace gsmlib;

struct IncomingMessage
{
  // used if new message is put into store
  int _index;                   // -1 means message want send directly
  string _storeName;
  // used if SMS message was sent directly to TA
  SMSMessageRef _newSMSMessage;
  // used if CB message was sent directly to TA
  CBMessageRef _newCBMessage;
  // used in both cases
  GsmEvent::SMSMessageType _messageType;

  IncomingMessage() : _index(-1) {}
};

class EventHandler : public GsmEvent
{
public:
  // inherited from GsmEvent
	void SMSReception(SMSMessageRef newMessage,SMSMessageType messageType);
	void CBReception(CBMessageRef newMessage);
	void SMSReceptionIndication(string storeName, unsigned int index,SMSMessageType messageType);
	EventHandler(){};
	virtual ~EventHandler() {}
	
	vector<IncomingMessage>& GetNewMessages() { return _newMessages; }
	int GetNumNewMsgs() const { return _newMessages.size();}

private:
	vector<IncomingMessage> _newMessages;
};

class CSMSListener : public JTCThread
{
public:
	CSMSListener();
	virtual ~CSMSListener();
	virtual void run();

private:
	bool _stop;
	void CheckForNewMessages(EventHandler& event_handler,MeTa* me);
};

#endif