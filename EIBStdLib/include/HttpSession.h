#ifndef __HTTP_SESSION_HEADER__
#define __HTTP_SESSION_HEADER__

#include <ctime>
#include <cstdlib>
#include <map>
#include "CString.h"
#include "Digest.h"
#include "CTime.h"
#include "JTC.h"

//forward declaration
class CSessionManager;
//session timeout (in minuets)
#define DEFAULT_SESSION_TIMEOUT 20

class EIB_STD_EXPORT CHttpSessionData
{
public:
	CHttpSessionData(int timeout = DEFAULT_SESSION_TIMEOUT);
	virtual ~CHttpSessionData();

	bool IsExpired() const;
	const CString& GetSessionID() const { return _session_id;}

	friend class CSessionManager;

private:
	CTime _expiration_time;
	CString _session_id;
};

class EIB_STD_EXPORT CSessionManager : public JTCMonitor
{
public:
	CSessionManager();
	virtual ~CSessionManager();

	bool FindSession(const CString& session_id);
	bool RemoveSession(const CString& session_id);
	void RegenerateSessionID(const CString& current,CString& new_session);
	void GenerateNewSession(const CString& user_name, CString& session_id);

private:
	map<CString,CHttpSessionData> _container;
};


#endif
