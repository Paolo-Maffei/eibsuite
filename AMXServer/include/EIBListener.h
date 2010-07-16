#ifndef __EIB_LISTENER_HEADER__
#define __EIB_LISTENER_HEADER__

#include "JTC.h"
#include "EIBAddress.h"
#include "MsgTable.h"
#include "EibNetwork.h"

using namespace EibStack;

class CEIBListener : public JTCThread, public JTCMonitor
{
public:
	CEIBListener();
	virtual ~CEIBListener();

	virtual void run();

	void Close();

private:
	bool _stop;
};

#endif

