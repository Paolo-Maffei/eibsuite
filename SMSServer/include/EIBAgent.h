#ifndef __EIB_AGENT_H__
#define __EIB_AGENT_H__

#include "JTC.h"
#include "GenericServer.h"

class CEIBAgent : public CGenericServer, public JTCThread
{
public:
	CEIBAgent();
	~CEIBAgent();

	virtual void run();
	bool ConnectToEIB();
	void Close();

private:
	bool _stop;
};

#endif
