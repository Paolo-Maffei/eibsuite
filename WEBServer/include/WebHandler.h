#ifndef __WEB_HANDLER_HEADER__
#define __WEB_HANDLER_HEADER__

#include <queue>
#include "CString.h"
#include "JTC.h"
#include "CMutex.h"
#include "Socket.h"
#include "HttpParser.h"
#include "Digest.h"
#include "StatsDB.h"
#include "ContentGenerator.h"
#include "UsersDB.h"

#define MAX_HTTP_REQUEST_SIZE 4096

class CWebHandler : public JTCThread, public JTCMonitor
{
public:
	CWebHandler();
	virtual ~CWebHandler();

	virtual void run();
	void AddToJobQueue(TCPSocket* job);
	void Close();
	void Signal();

private:
	void HandleRequest(TCPSocket* sock, char* buffer,CHttpReply& reply);
	void RemoveFromJobQueue();
	void GenerateFirstContent(CString& content);
	void InitReply(CHttpReply& reply);
	void GetHisotryFromEIB(CStatsDB& db,CString& err);

	void CreateContent(CHttpRequest& request, CHttpReply& reply, const CUser& user);
	//bool SendEIBCommand(const CString& addr, unsigned int apci,CString& err);
	bool SendEIBCommand(const CString& addr, unsigned char *apci, unsigned char apci_len, CString& err);

	inline const CString& GetCurrentDomain() const;
	bool GetByteArrayFromHexString(const CString& str, unsigned char *val, unsigned char &val_len);

	void HandleFavoritsIconRequest(CHttpReply& reply);
	void HandleImageRequest(const CString& file_name, CHttpReply& reply);
	void FillRawFile(const CString& file_name, CDataBuffer& buf, int& total_size);

private:
	queue<TCPSocket*> _job_queue;
	bool _stop;

	unsigned char HexToChar(const CString& hexNumber);
	int	GetDigitValue (char digit);
};

#endif
