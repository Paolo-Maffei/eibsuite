#ifndef __AMX_HANDLER_HEADER__
#define __AMX_HANDLER_HEADER__

#include "JTC.h"
#include "Socket.h"
#include "AMXServerConfig.h"
#include "CRC.h"
#include "CMutex.h"

typedef struct ParseResult
{
	bool is_valid;
	int length;
	int channel_id;
	bool on;
}ParseResult;

class CAMXListener : public JTCThread, JTCMonitor
{
public:
	CAMXListener();
	virtual ~CAMXListener();
	void Close();
	virtual void run();
	void Init();

	void SendAMXMessage(unsigned char device_id, bool press);

private:
	bool ParseSingleAMXFrame(unsigned char* buf, int length, ParseResult& res);
	void MaintainAMXConnection(unsigned char* buffer, int& len);
	void HandleParsedFrame(const ParseResult& result);

private:
	TCPSocket _sock;
	bool _stop;
	JTCMonitor _mon;
};

#endif
