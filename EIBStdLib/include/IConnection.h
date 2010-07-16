#ifndef __ICONNECTION_HEADER__
#define __ICONNECTION_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "CemiFrame.h"
#include "EibNetwork.h"
#include "Monitor.h"

using namespace EibStack;

typedef EIB_STD_EXPORT struct _KnxElementQueue
{
	CCemiFrame _frame;
	BlockingMode _mode;
	JTCMonitor* _optional_mon;
}KnxElementQueue;

class EIB_STD_EXPORT IConnection
{
public:
	IConnection();
	virtual ~IConnection();
	virtual bool Connect() = 0;
	virtual bool IsConnected() const = 0;
	virtual void DisConnect() = 0;

	virtual bool SendDataFrame(const KnxElementQueue& frame) = 0;
	virtual bool ReceiveDataFrame(CCemiFrame& frame) = 0;

	virtual const CString& GetDeviceDataAddress() = 0;
	virtual int GetDeviceDataPort() = 0;

	virtual const CString& GetDeviceControlAddress() = 0;
	virtual int GetDeviceControlPort() = 0;

	virtual int GetLocalPort() = 0;
};

#endif
