#ifndef __SMS_SERVER_HEADER_
#define __SMS_SERVER_HEADER_

#include "Globals.h"
#include "gsm_config.h"
#include "gsm_nls.h"
#include "gsm_error.h"
#include "gsm_me_ta.h"
#include "gsm_util.h"
#include "JTC.h"
#include "CMutex.h"
#include "GenericServer.h"
#include "SMSServerConfig.h"
#include "SMSServerDB.h"
#include "LogFile.h"
#include "MsgTable.h"
#include "SingletonValidation.h"
#include "SMSListener.h"

#ifdef WIN32
#include "gsm_win32_serial.h"
#include <direct.h>
typedef Win32SerialPort SerialPort;
#else
#include "gsm_unix_serial.h"
typedef UnixSerialPort SerialPort;
#define _MAX_PATH 256
#endif


using namespace gsmlib;
using namespace std;

#define DEFAULT_LOG_FILE_NAME "SmsServer.log"

#define SMS_SERVER_PROCESS_NAME "SMSServer"

class CSMSServer : public CGenericServer, public JTCThread, public CSingletonProcess
{
public:
	CSMSServer();
	virtual ~CSMSServer();
	virtual void run();
	bool Init();
	void Close();

	CSMSServerDB& GetDB() { return _db;}

	/*!
		\fn static CSMSServer& GetInstance()
		\brief Returns reference to instance of CSMSServer
		Since this class is a singleton, any use of its member functions requires getting a reference to 
		the single instance of CSMSServer. This is the function that allows this.
	*/
	static CSMSServer& GetInstance() { return _instance;}
	CLogFile& GetLog() { return _log;}

	MeTa* GetMeTa() { return _meta; }
	SerialPort* GetSerialPort() { return _cell_port; }
	CMutex& GetMetaLock() { return _meta_lock; }
private:
	bool SendSMS(const CString& phone_number, const CString& text);
	void DeleteAllMessages();

private:
	static CSMSServer _instance;
	CSMSServerConfig _conf;
	CSMSServerDB _db;
	SerialPort* _cell_port;
	CLogFile _log;
	CMsgTable _msg_table;
	CSMSListener _listener;
	MeTa* _meta;
	CMutex _meta_lock;
	bool _stop;
};

#endif

